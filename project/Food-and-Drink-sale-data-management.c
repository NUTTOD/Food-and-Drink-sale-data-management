#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_symbol(int length, char symbol){
    for (int i = 0; i < length; i++){
        putchar(symbol);
    }
    putchar('\n');
}

int ReadCSV (){
    char buffer [1024];
    FILE *CSVFile = fopen("csvfile/Food-and-Drink-sale-data-management.csv", "r");

    if (CSVFile == NULL){
        printf("File %p\n", CSVFile);
        printf("เปิดไฟล์ไม่สำเร็จ\n");
    }
    else{
        printf("File %p\n", CSVFile);
        printf("เปิดไฟล์สำเร็จ\n");

        if (fgets(buffer, sizeof(buffer), CSVFile) != NULL){
            char *header_token = strtok(buffer, ",\n");
            while (header_token != NULL){
                printf("%-35s", header_token);
                header_token = strtok(NULL, ",\n");
            }
            printf("\n");
            print_symbol(120, '=');
        }

        fclose(CSVFile);
    }

}

int main(){
    int menu = 0;

    while(menu != 7){
        printf("ระบบจัดการข้อมูลการซื้ออาหารและเครื่องดื่ม\n");
        printf("------------โปรดเลือกฟังก์ชันที่ต้องการ------------\n");
        printf("1.อ่านข้อมูลการซื้อ\n");
        printf("2.บันทึกข้อมูลการซื้อ\n");
        printf("3.เพิ่มข้อมูลการซื้อ\n");
        printf("4.ค้นหาข้อมูลการซื้อ\n");
        printf("5.อัพเดตข้อมูลการซื้อ\n");
        printf("6.ลบข้อมูลการซื้อ\n");
        printf("7.ออกจากโปรแกรม");
        printf("เลือกฟังก์ชันโดยตัวเลข 1-7 : ");
        scanf("%d", &menu);
        print_symbol(44, '-');

        switch (menu){
            case 1: 
                ReadCSV();
                print_symbol(44, '-');
                break;
            case 2:
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
        }
    }
    return 0;
}