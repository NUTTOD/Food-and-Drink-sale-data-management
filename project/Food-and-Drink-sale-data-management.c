#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define MAXFILE 50

void print_symbol(int length, char symbol);
int getLastorderID(char *filepath);
char findFilelist(char *filelist[], int *filecount);
int SelectCSVFile(char *selected_path, int buffer_size);
void DisplayCSVContent(const char *filepath);

int ReadCSV();
int SaveCSV();
int AddCSV();
int SearchCSV();
int UpdateCSV();
int DeleteCSV();

void print_symbol(int length, char symbol){
    for (int i = 0; i < length; i++){
        putchar(symbol);
    }
    putchar('\n');
}

int getLastorderID(char *filepath){
    FILE *CSVFile = fopen(filepath, "r");
    char line[1024];
    int lastOrderID = 0;
    int currentID = 0;

    if (CSVFile == NULL){
        printf("เปิดไฟล์ไม่สำเร็จหรือไม่พบไฟล์\n");
        return 0;
    }
    else{
        while (fgets(line, sizeof(line), CSVFile) != NULL){
            if (sscanf(line, "%d,", &currentID) == 1){
                lastOrderID = currentID;
            }
        }
        fclose(CSVFile);
    }
    return lastOrderID;
}

char findFilelist(char *filelist[], int *filecount){
    WIN32_FIND_DATA findData;
    HANDLE hFind;
    hFind = FindFirstFile("csvfile\\*.*", &findData);

    char filename [1024];

    if (hFind == INVALID_HANDLE_VALUE){
        printf("ไม่เจอ path หรือ error");
        return 1;
    }

    do{
        if (strcmp(findData.cFileName, ".") != 0 && strcmp(findData.cFileName, "..") != 0){
            if (*filecount < MAXFILE){
                size_t len = strlen(findData.cFileName) + 1;
                filelist[*filecount] = (char *)malloc(len);

                if (filelist[*filecount] != NULL){
                    strcpy(filelist[*filecount], findData.cFileName);
                    (*filecount) ++;
                }else{
                    printf("Memory allocation failded\n");
                }
            }
        }
    }while(FindNextFile(hFind, &findData) != 0);
    FindClose(hFind);
}

int SelectCSVFile(char *selectedPath, int buffer_size){
    char *filelist[MAXFILE];
    int filecount = 0;
    findFilelist(filelist, &filecount);

    if (filecount == 0){
        printf("ไม่พบไฟล์ CSV ในโฟลเดอร์ csvfile\n");
    }

    printf("โปรดเลือกไฟล์ที่ต้องการค้นหาข้อมูล\n");
    for (int i = 0; i < filecount; i++){
        printf("%d.%s\n", i + 1, filelist[i]);
    }
    print_symbol(44,'-');

    int filechoice = 0;
    printf("เลือกโดยใช้ตัวเลขตามหัวข้อ : ");
    int scancheck = scanf("%d", &filechoice);
    getchar();

    int success = 0;
    if (scancheck != 1 || filechoice < 1 || filechoice > filecount){
        printf("ตัวเลือกไม่ถูกต้อง\n");
        while(getchar() != '\n')
        success = 0;
    }
    else{
        snprintf(selectedPath, buffer_size, "csvfile\\%s", filelist[filechoice - 1]);
        success = 1;
    }

    for (int i = 0; i < filecount; i++){
        free(filelist[i]);
    }
    
    return success;
}

int UpdateCSV(){
    int orderIDchoice = 0;
    ReadCSV();
    printf("โปรดเลือกสินค้าที่ต้องการแก้ไข้ข้อมูลจาก OrderID : ");
    int scancheck = scanf("%d", &orderIDchoice);

    if (scancheck == 1){
        printf("%d\n", orderIDchoice);
    }
    else{
        printf("ตัวเลือกไม่ถูกต้อง\n");
        while (getchar() != '\n');
        print_symbol(44, '-');
    }

    return 1;
}

int SearchCSV(){
    char filename [1024];
    char *filelist[MAXFILE];
    int filecount = 0;
    findFilelist(filelist, &filecount);

    int filechoice = 0;
    printf("โปรดเลือกไฟล์ที่ต้องการค้นหาข้อมูล\n");
    for (int i = 0; i < filecount; i++){
        printf("%d.%s\n", i + 1, filelist[i]);
    }

    printf("เลือกโดยใช้ตัวเลขตามหัวข้อ : ");
    int scancheck = scanf("%d", &filechoice);
    print_symbol(44,'-');
    getchar();

    if (scancheck != 1 || filechoice < 1 || filechoice > filecount){
        printf("ตัวเลือกไม่ถูกต้อง\n");
        while (getchar() != '\n');
    }
    else{
        int searchType = 0;

        printf("โปรดเลือกประเภทการค้นหา\n");
        printf("1.ค้นหาด้วย ID\n2.ค้นหาด้วยชื่อ\n");
        printf("โปรดเลือกตัวเลือกตามหัวข้อ : ");
        scancheck = scanf("%d", &searchType);
        getchar();
        if (scancheck == 1){
            if (searchType != 1 && searchType != 2){
                printf("ตัวเลือกผิดพลาด\n");
                if (scancheck != 1) {
                    while (getchar() != '\n');
                }
            }
            else{
                char path[1024] = "csvfile\\";
                strcat(path, filelist[filechoice - 1]);

                FILE *CSVFile = fopen(path, "r");
                switch (searchType){
                    case 1:{
                        int found = 0;
                        int searchID;
                        char searchIDLine[1024];
                        char Linecopy[1024];

                        printf("โปรดใส่หมายเลข ID ที่ต้องการค้นหา : ");
                        scanf("%d", &searchID);
                        getchar();
                        while (fgets(searchIDLine, sizeof(searchIDLine), CSVFile) != NULL){
                            int currentId;
                            strcpy(Linecopy, searchIDLine);
                            if (sscanf(searchIDLine, "%d,", &currentId) == 1) {
                                if (currentId == searchID) {
                                    char *token = strtok(Linecopy, ",\n");
                                    print_symbol(44, '-');
                                    printf("พบสินค้ารหัส : %d\n", currentId);
                                    print_symbol(155, '=');
                                    printf("%-31s%-31s%-31s%-31s%-31s|\n", "OrderID", "ProductName", "Quantity", "Price", "Type");
                                    while (token != NULL){
                                        printf("%-30s ", token);
                                        token = strtok(NULL, ",\n");
                                    }
                                    printf("|\n");
                                    print_symbol(155, '=');

                                    found = 1;
                                    break;
                                }
                            }
                        }
                        if (found == 0){
                            print_symbol(44, '-');
                            printf("ไม่พบรหัสสินค้าที่ค้นหา\n");
                            print_symbol(44, '-');
                        }
                        break;
                    }
                    case 2:{
                        int found = 0;
                        char searchName[1024];
                        char searchNameLine[1024];
                        printf("โปรดใส่ \"ชื่อ\" \"หรือประเภท\" ที่ต้องการค้นหา : ");
                        scanf("%s", searchName);
                        getchar();
                        print_symbol(155, '=');
                        printf("%-31s%-31s%-31s%-31s%-31s|\n", "OrderID", "ProductName", "Quantity", "Price", "Type");
                        int productCount = 0;
                        while (fgets(searchNameLine, sizeof(searchNameLine), CSVFile) != NULL){
                            if (strstr(searchNameLine, searchName) != NULL){
                                char *token = strtok(searchNameLine, ",\n");
                                productCount++;
                                while (token != NULL){
                                    printf("|%-30s", token);
                                    token = strtok(NULL, ",\n");
                                }
                                printf("|\n"); 
                                found = 1;
                            }
                        }
                        print_symbol(155, '=');
                        printf("เจอสินค้า %d อย่างจากคำค้นหา %s\n", productCount, searchName);
                    }
                    fclose(CSVFile);
                    for (int i = 0; i < filecount; i++){
                        free(filelist[i]);
                    }
                    break;
                }
            }
        }
        else{
            print_symbol(44, '-');
            printf("ตัวเลือกผิดพลาด\n");
            while(getchar() != '\n');
            print_symbol(44, '-');
            print_symbol(3, '\n');
        }
    }
    return 1;
}

int AddCSV (){
    char filename [1024];
    char *filelist[MAXFILE];
    int filecount = 0;
    findFilelist(filelist, &filecount);

    int choice = 0;
    printf("โปรดเลือกไฟล์ที่ต้องการเพิ่มข้อมูล\n");
    for (int i = 0; i < filecount; i++){
        printf("%d.%s\n", i + 1, filelist[i]);
    }
    print_symbol(44, '-');
    printf("เลือกโดยใช้ตัวเลขตามหัวข้อ : ");
    scanf("%d", &choice);
    getchar();
    if (choice < 1 || choice > filecount){
        printf("ตัวเลือกไม่ถูกต้อง\n");
    }
    else{
        strcpy(filename, filelist[choice - 1]);
        char path[1024] = "csvfile\\";
        strcat(path, filename);
        FILE *CSVFile = fopen(path, "a");

        if (CSVFile == NULL){
            printf("เปิดไฟล์ไม่สำเร็จหรือไม่พบไฟล์\n");
        }
        else{
            int choice = 1;
            int orderID = getLastorderID(path) + 1;
            do{
                char name [100];
                int quatity;
                int price;
                int productTypechoice = 0;
                char productType [10];

                printf("ใส่ชื่อรายการที่ %d : ", orderID);
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;

                printf("ใส่จำนวน: ");
                scanf("%d", &quatity);

                printf("ใส่ราคา: ");
                scanf("%d", &price);

                printf("ประเภท\n1.Food\n2.Drink\nโปรดเลือกด้วตามหัวข้อ : ");
                scanf("%d", &productTypechoice);

                if (productTypechoice == 1){
                    strcpy(productType, "Food");
                }
                else if (productTypechoice == 2){
                    strcpy(productType, "Drink");
                }
                else{
                    strcpy(productType, "None");
                }

                fprintf(CSVFile, "%04d,%s,%d,%d,%s\n", orderID, name, quatity, price, productType);

                printf("\nต้องการเพิ่มรายการต้อไปหรือไม่ (1 = ใช่, 0 = ไม่ใช่): ");
                scanf("%d", &choice);
                print_symbol(44, '-');

                getchar();

                orderID++;
                printf("\n");

            }while (choice != 0);
        }
    fclose(CSVFile);
    }

    for (int i = 0; i < filecount; i++){
        free(filelist[i]);
    }
    return 1;
}

int SaveCSV (char *filename){
    char path[1024] = "csvfile\\";
    strcat(path, filename);
    strcat(path, ".csv");

    FILE *checkFile = fopen(path, "r");
    if (checkFile != NULL){
        printf("มีไฟล์ชื่อนี้อยู่แล้ว");
        fclose(checkFile);
        return 0;
    }
    else{
        FILE *CSVFile = fopen(path, "a");
        if (CSVFile == NULL){
            printf("ไม่สามารถสร้างไฟล์ได้\n");
        }
        else{
            printf("สร้างไฟล์ %s.csv สำเร็จ\n", filename);
            fprintf(CSVFile, "OrderID,ProductName,Quantity,Price,Type\n");
            int choice = 1;
            int orderID = 1;
            do{
                char name [100];
                int quatity;
                int price;
                int productTypechoice = 0;
                char productType [10];

                printf("ใส่ชื่อรายการที่ %d : ", orderID);
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;

                printf("ใส่จำนวน: ");
                scanf("%d", &quatity);

                printf("ใส่ราคา: ");
                scanf("%d", &price);

                printf("ประเภท\n1.Food\n2.Drink\nโปรดเลือกด้วตามหัวข้อ : ");
                scanf("%d", &productTypechoice);

                if (productTypechoice == 1){
                    strcpy(productType, "Food");
                }
                else if (productTypechoice == 2){
                    strcpy(productType, "Drink");
                }
                else{
                    strcpy(productType, "None");
                }

                fprintf(CSVFile, "%04d,%s,%d,%d,%s\n", orderID, name, quatity, price, productType);

                printf("\nต้องการเพิ่มรายการต้อไปหรือไม่ (1 = ใช่, 0 = ไม่ใช่): ");
                scanf("%d", &choice);

                getchar();

                orderID++;
                printf("\n");

            }while (choice != 0);
        }
        fclose(CSVFile);
    }
    return 1;
}

int ReadCSV (){
    char buffer[1024];
    char filename [1024];
    char *filelist[MAXFILE];
    int filecount = 0;
    findFilelist(filelist, &filecount);

    printf("โปรดเลือกไฟล์ที่ต้องการจะเปิด\n");
    for (int i = 0; i < filecount; i++){
        printf("%d.%s\n", i + 1, filelist[i]);
    }
    print_symbol(44, '-');

    int choice = 0;
    printf("เลือกโดยใช้ตัวเลขตามหัวข้อ : ");
    scanf("%d", &choice);
    if (choice < 1 || choice > filecount){
        printf("ตัวเลือกไม่ถูกต้อง");
    }
    else{
        strcpy(filename, filelist[choice - 1]);
        char path[1024] = "csvfile\\";
        strcat(path, filename);
        FILE *CSVFile = fopen(path, "r");

        if (CSVFile == NULL){
            printf("เปิดไฟล์ไม่สำเร็จหรือไม่พบไฟล์\n");
        }
        else{
            printf("เปิดไฟล์ %s สำเร็จ\n", filename);
            if (fgets(buffer, sizeof(buffer), CSVFile) != NULL){
                char *header_token = strtok(buffer, ",\n");
                while (header_token != NULL){
                    printf("%-31s", header_token);
                    header_token = strtok(NULL, ",\n");
                }
                printf("\n");
                print_symbol(155, '=');
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
            print_symbol(155, '=');
            fclose(CSVFile);
        }   
    }
    for (int i = 0; i < filecount; i++){
        free(filelist[i]);
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
        printf("7.ออกจากโปรแกรม\n");
        printf("8.Unit Test\n");
        printf("9.End-to-End Test\n");
        printf("เลือกฟังก์ชันโดยตัวเลข 1-9 : ");
        scan_check = scanf("%d", &menu);
        getchar();

        if (scan_check == 1){
            print_symbol(44, '-');
            switch (menu){
                case 1: 
                    ReadCSV();
                    break;
                case 2:{
                    char filename[1024];
                    printf("โปรดระบุชื่อไฟล์ที่ต้องการสร้าง : ");
                    scanf("%s", filename);
                    getchar();            
                    SaveCSV(filename);
                    break;}
                case 3:
                    AddCSV();
                    break;
                case 4:
                    SearchCSV();
                    break;
                case 5:
                    UpdateCSV();
                    break;
                case 6:
                    break;
                case 7:
                    printf("จบการทำงานของโปรแกรม\n");
                    print_symbol(44, '-');
                    break;
                case 8:
                    break;
                case 9:
                    break;
                default:
                    printf("โปรดเลือกตัวเลือกที่ 1-9\n");
                    menu = 0;
                    print_symbol(44, '-');
                    break;
            }
            print_symbol(3, '\n');
        }
        else{
            print_symbol(44, '-');
            printf("โปรดเลือกตัวเลือกที่ 1-9\n");
            while(getchar() != '\n');
            menu = 0;
            print_symbol(44, '-');
            print_symbol(3, '\n');
        }
    }
    return 1;
}