#include <stdio.h>
#include <string.h>

void print_symbol(int length, char symbol){
    for (int i = 0; i < length; i++){
        putchar(symbol);
    }
    putchar('\n');
}

int SaveCSV (char *filename){
    char path[1024] = "csvfile/";
    char filetype[] = ".csv";
    strcat(path, filename);
    strcat(path, filetype);
    FILE *CSVFile = fopen(path, "a");
}

int ReadCSV (){
    char buffer [1024];
    char filename [] = "csvfile/Food-and-Drink-sale-data-management.csv";
    FILE *CSVFile = fopen(filename, "r");

    if (CSVFile == NULL){
        printf("ที่อยู่ %p\n", CSVFile);
        printf("เปิดไฟล์ไม่สำเร็จหรือไม่พบไฟล์\n");
    }
    else{
        //printf("File %p\n", CSVFile);
        printf("เปิดไฟล์สำเร็จ\n");

        if (fgets(buffer, sizeof(buffer), CSVFile) != NULL){
            char *header_token = strtok(buffer, ",\n");
            while (header_token != NULL){
                printf("%-31s", header_token);
                header_token = strtok(NULL, ",\n");
            }
            printf("\n");
            print_symbol(125, '=');
        }
        while (fgets(buffer, sizeof(buffer), CSVFile) != NULL){
            char *token = strtok(buffer, ",\n");
            while (token != NULL){
                printf("|%-30s", token);
                token = strtok(NULL, ",\n");
            }
            printf("|");
            printf("\n");
        }

        fclose(CSVFile);
    }
    return 1;
}

int main(){
    int menu = 0;
    int scan_check;

    while(menu != 7){
        printf("     ระบบจัดการข้อมูลการซื้ออาหารและเครื่องดื่ม\n");
        printf("------------โปรดเลือกฟังก์ชันที่ต้องการ------------\n");
        printf("1.อ่านข้อมูลการซื้อ\n");
        printf("2.บันทึกข้อมูลการซื้อ\n");
        printf("3.เพิ่มข้อมูลการซื้อ\n");
        printf("4.ค้นหาข้อมูลการซื้อ\n");
        printf("5.อัพเดตข้อมูลการซื้อ\n");
        printf("6.ลบข้อมูลการซื้อ\n");
        printf("7.ออกจากโปรแกรม");
        printf("เลือกฟังก์ชันโดยตัวเลข 1-7 : ");
        scan_check = scanf("%d", &menu);

        if (scan_check == 1){
            print_symbol(44, '-');
            switch (menu){
                case 1: 
                    ReadCSV();
                    print_symbol(125, '=');
                    break;
                case 2:
                    char filename[1024];
                    printf("โปรดระบุชื่อไฟล์ที่ต้องการ : ");
                    scanf("%s", &filename);
                    SaveCSV(filename);
                    break;
                case 3:
                    break;
                case 4:
                    break;
                case 5:
                    break;
                case 6:
                    break;
                case 7:
                    printf("จบการทำงานของโปรแกรม\n");
                    break;
                default:
                    printf("โปรดเลือกตัวเลือกที่ 1-7\n");
                    menu = 0;
                    print_symbol(44, '-');
                    break;
            }
            print_symbol(3, '\n');
        }
        else{
            print_symbol(44, '-');
            printf("โปรดเลือกตัวเลือกที่ 1-7\n");
            while(getchar() != '\n');
            menu = 0;
            print_symbol(44, '-');
            print_symbol(3, '\n');
        }

    }

    return 0;
}