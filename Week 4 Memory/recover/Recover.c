#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // 排除錯誤的 argument
    if (argc != 2)
    {
        printf("Usage: ./recover FILE\n");
        return 1;
    }

    // 開啟檔案
    FILE *card = fopen(argv[1], "r");

    // 若開啟失敗，return 1
    if (card == NULL)
    {
        printf("Can't open the file.\n");
        return 1;
    }

    // 讀取：只要還有剩餘資料就繼續讀取
    int BLOCKSIZE = 512;
    uint8_t buffer[BLOCKSIZE];
    int counter = 0;
    char *filename = malloc(8 * sizeof(char));
    FILE *img = NULL;

    while (fread(buffer, sizeof(uint8_t), BLOCKSIZE, card) == BLOCKSIZE)
    {
        // 讀取 JPEGs 開頭
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // 如果上一個檔案存在，先關掉檔案
            if (img != NULL)
            {
                fclose(img);

                // 計數
                counter++;
            }

            // 建立檔案
            // 命名
            sprintf(filename, "%03i.jpg", counter);

            // 建立檔案，並確認檔案存在
            img = fopen(filename, "w");
            if (img == NULL)
            {
                printf("Could not create %s\n", filename);
                return 1;
            }

            // 寫入檔案
            fwrite(buffer, sizeof(uint8_t), BLOCKSIZE, img);
        }
        else if (img != NULL)
        {
            // 寫入檔案
            fwrite(buffer, sizeof(uint8_t), BLOCKSIZE, img);
        }
    }

    // 關閉所有檔案
    fclose(card);
    fclose(img);
    free(filename);
    return 0;
}
