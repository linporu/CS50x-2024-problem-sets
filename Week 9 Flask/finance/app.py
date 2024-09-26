import os
from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash
from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Check if user exists
    if "user_id" not in session:
        return redirect("/login")
    if not db.execute("SELECT * FROM users WHERE id = ?", session["user_id"]):
        session.clear()
        return redirect("/login")

    # Set variables
    user_id = session["user_id"]
    cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]
    stocks = db.execute("SELECT symbol, quantity FROM positions WHERE user_id = ? and quantity > 0", user_id)

    # Calculate total stock value and stock value percentages
    total_stock_value = 0

    for stock in stocks:
        quote = lookup(stock["symbol"])
        if quote:
            stock["price"] = quote["price"]
            stock["total_amount"] = stock["quantity"] * stock["price"]
            total_stock_value += stock["total_amount"]
            
    for stock in stocks:
        stock["percentage"] = (stock["total_amount"] / total_stock_value) * 100

    # Calculate Account value
    account_value = cash + total_stock_value

    # Calculate cash percentage and total stock percentage
    cash_percentage = (cash / account_value) * 100
    total_stock_percentage = (total_stock_value / account_value) * 100

    # Set table header
    headers = ["Symbol", "Quantity", "Price", "Total Amount", "Percentage"]

    return render_template("index.html", 
                           account_value=usd(account_value),
                           total_stock_value=usd(total_stock_value),
                           total_stock_percentage=f"{total_stock_percentage:.2f}%",
                           headers=headers,
                           stocks=stocks,
                           cash=usd(cash),
                           cash_percentage=f"{cash_percentage:.2f}%")


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":

        # Check symbol
        if not request.form.get("symbol"):
            return apology("must provide stock symbol", 400)
        if not lookup(request.form.get("symbol")):
            return apology("Invalid symbol", 400)
        
        # Check shares exist
        if not request.form.get("shares"):
            return apology("Must provide shares", 400)
        
        # Check number of shares is integer
        try:
            num_shares = float(request.form.get("shares"))
            if num_shares.is_integer() and num_shares > 0:
                pass
            else:
                return apology("Shares must be an integer greater than 0", 400)
        except ValueError:
            return apology("Shares must be an integer greater than 0", 400)
        
        # Set variables
        user_id = session["user_id"]
        stock_bought = lookup(request.form.get("symbol"))
        symbol = stock_bought["symbol"]
        quantity = int(request.form.get("shares"))
        price = stock_bought["price"]
        total_amount = price * quantity
        cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]

        # Check cash is enough
        if cash < total_amount:
            return apology("Cash is not enough")

        # Record transaction
        db.execute("INSERT INTO transactions (user_id, transaction_type, symbol, quantity, price, total_amount) VALUES (?, ?, ?, ?, ?, ?)", 
                   user_id, "BUY", symbol, quantity, price, total_amount)
        
        # Update cash
        db.execute("UPDATE users SET cash = cash - ? WHERE id = ?", total_amount, user_id)
       
        # Update position
        # Check symbol is existing first
        existing = db.execute("SELECT * FROM positions WHERE user_id = ? AND symbol = ?", user_id, symbol)
        if existing:
            db.execute("UPDATE positions SET quantity = quantity + ? WHERE user_id = ? AND symbol = ?", quantity, user_id, symbol)
        else:
            db.execute("INSERT INTO positions (user_id, symbol, quantity) VALUES (?, ?, ?)", user_id, symbol, quantity)

        return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Set variables
    user_id = session["user_id"]
    transactions = db.execute("""
                              SELECT transaction_date, transaction_type, symbol, quantity, price, total_amount 
                              FROM transactions 
                              WHERE user_id = ? 
                              ORDER BY transaction_date DESC
                              """, user_id)
    
    # Set table header
    headers = ["Transaction date", "Transaction type", "Symbol", "Quantity", "Price", "Total amount"]

    return render_template("history.html", 
                            transactions=transactions, 
                            headers=headers
                            )


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("must provide stock symbol", 400)
        quote = lookup(request.form.get("symbol"))
        if not quote:
            return apology("Invalid symbol", 400)
        
        return render_template("quoted.html", quote=quote)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        if not request.form.get("username"):
            return apology("must provide username", 400)
        elif not request.form.get("password"):
            return apology("must provide password", 400)
        elif not request.form.get("confirmation"):
            return apology("must provide confirmation", 400)
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords do not match", 400)
        
        hash = generate_password_hash(request.form.get("password"))
        
        try:
            db.execute("INSERT INTO users (username, hash, registered_at) VALUES (?, ?, CURRENT_TIMESTAMP)", request.form.get("username"), hash)
            return redirect("/login")
        except:
            return apology("username already exists", 400)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":

        # Check symbol
        if not request.form.get("symbol"):
            return apology("must provide stock symbol", 403)
        if not lookup(request.form.get("symbol")):
            return apology("Invalid symbol", 403)
        
        # Check if position exists 
        user_positions = db.execute("SELECT symbol FROM positions WHERE user_id = ?", session["user_id"])  # 避免 user 無資料時 IndexError
        if not user_positions or request.form.get("symbol") not in [position["symbol"] for position in user_positions]:
            return apology("No such position", 403)

        # Check if number of shares exists
        if not request.form.get("shares"):
            return apology("Must provide shares")   

        # Check number of shares is integer
        try:
            num_shares = float(request.form.get("shares"))
            if num_shares.is_integer() and num_shares > 0:
                pass
            else:
                return apology("Shares must be an integer greater than 0", 400)
        except ValueError:
            return apology("Shares must be an integer greater than 0", 400)

        # Set variables
        user_id = session["user_id"]
        stock_to_sell = lookup(request.form.get("symbol"))
        symbol = stock_to_sell["symbol"]
        quantity = int(request.form.get("shares"))
        price = stock_to_sell["price"]
        total_amount = price * quantity
        position = db.execute("SELECT quantity FROM positions WHERE user_id = ? AND symbol = ?", user_id, symbol)[0]["quantity"]

        # Check shares are enough
        if position < quantity:
            return apology("Not enough share")

        # Record transaction
        db.execute("INSERT INTO transactions (user_id, transaction_type, symbol, quantity, price, total_amount) VALUES (?, ?, ?, ?, ?, ?)", 
                   user_id, "SELL", symbol, quantity, price, total_amount)
        
        # Update cash
        db.execute("UPDATE users SET cash = cash + ? WHERE id = ?", total_amount, user_id)
       
        # Update position
        db.execute("UPDATE positions SET quantity = quantity - ? WHERE user_id = ? AND symbol = ?", quantity, user_id, symbol)
        
        return redirect("/")
    
    else:
        # Set variables
        user_id = session["user_id"]
        stocks = db.execute("SELECT symbol FROM positions WHERE user_id = ? AND quantity > 0", user_id)

        return render_template("sell.html", stocks=stocks)
    