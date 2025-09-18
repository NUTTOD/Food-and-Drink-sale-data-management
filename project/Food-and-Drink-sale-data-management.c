#include <stdio.h>



int main(){
    int menu;
    while(menu!=7){
        printf("ระบบจัดการข้อมูลการซื้ออาหารและเครื่องดื่ม\n");
        printf("---------โปรดเลือกฟังก์ชันที่ต้องการ---------\n");
        printf("1.อ่านข้อมูลการซื้อ\n");
        printf("2.บันทึกข้อมูลการซื้อ\n");
        printf("3.เพิ่มข้อมูลการซื้อ\n");
        printf("4.ค้นหาข้อมูลการซื้อ\n");
        printf("5.อัพเดตข้อมูลการซื้อ\n");
        printf("6.ลบข้อมูลการซื้อ\n");
        printf("7.ออกจากโปรแกรม");
        printf("-------------------------------------\n");
        printf("เลือกฟังก์ชันโดยตัวเลข 1-7 : ");
        scanf("%d", &menu);

        switch (menu){
            case 1:
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
            default:
        }
    }
    return 0;
}