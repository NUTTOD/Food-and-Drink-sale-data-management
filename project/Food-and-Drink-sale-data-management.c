#include <stdio.h>

int ReadCSV (){
    FILE *CSVFile = fopen("csvfile/Food-and-Drink-sale-data-management.csv", "r");
    if (CSVFile == NULL){
        printf("File %p\n", CSVFile);
        printf("เปิดไฟล์ไม่สำเร็จ\n");
    }
    else{
        printf("File %p\n", CSVFile);
        printf("เปิดไฟล์สำเร็จ\n");
        fclose(CSVFile);
    }
}

int main(){
    int menu = 0;
    while(menu != 7){
        printf("ระบบจัดการข้อมูลการซื้ออาหารและเครื่องดื่ม\n");
        printf("---------โปรดเลือกฟังก์ชันที่ต้องการ---------\n");
        printf("1.อ่านข้อมูลการซื้อ\n");
        printf("2.บันทึกข้อมูลการซื้อ\n");
        printf("3.เพิ่มข้อมูลการซื้อ\n");
        printf("4.ค้นหาข้อมูลการซื้อ\n");
        printf("5.อัพเดตข้อมูลการซื้อ\n");
        printf("6.ลบข้อมูลการซื้อ\n");
        printf("7.ออกจากโปรแกรม");
        printf("เลือกฟังก์ชันโดยตัวเลข 1-7 : ");
        scanf("%d", &menu);
        printf("-------------------------------------\n");

        switch (menu){
            case 1: 
                ReadCSV();
                printf("-------------------------------------\n");
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