
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


struct User
{
    char name[100];
    char email[50];
    char password[30];
    int isLogin;
    double balance;
    int productCount;
};

struct Product
{
    char name[100];
    double price;
    int quantity;
};

struct Transaction
{
    double price;
    int quantity;
    char details[1000];
};



void showMenuList(void);
void productEntry(void);
void showProducts(void);
void saveProducts(struct Product *p, int items);

void saveTransaction(struct Transaction p);
void saveTransactions(struct Transaction *p, int items);

void productAdd(void);
void productRestore(void);
void sellProduct(void);
void showReceipt(struct Transaction trans);


void showMenuList(void);
void userLogout(void);
void showAccount(void);

int isFileEmpty(char *fileName);



#define ProductFile "ProductStore.bin"
#define TransactionFile "Transaction.bin"
#define UserFile "User.bin"

//#define ProductFile "C:\\My-Shop\\ProductStore.bin"
//#define TransactionFile "C:\\My-Shop\\Transaction.bin"
//#define UserFile "C:\\My-Shop\\User.bin"



void checkOrCreateFiles(){
    FILE *fp;
    
    if((fp = fopen(UserFile,"rb")) == NULL){
        fp = fopen(UserFile,"wb");
    }
    
    if((fp = fopen(ProductFile,"rb")) == NULL){
        fp = fopen(ProductFile,"wb");
    }
    
    if((fp = fopen(TransactionFile,"rb")) == NULL){
        fp = fopen(TransactionFile,"wb");
    }
    
    fclose(fp);
}

int isFileEmpty(char *fileName){
    FILE *fp = fopen(fileName,"rb");
    
    fseek(fp, 0, SEEK_END);
    unsigned long len = (unsigned long)ftell(fp);
    fclose(fp);
    
    return len>0 ? 0 :1;
}


void saveUser(struct User u){
    FILE *fp = fopen(UserFile,"wb");
    fwrite(&u, sizeof(struct User), 1, fp);
    fclose(fp);
}

struct User getUser(){
    struct User user;
    
    FILE *fp = fopen(UserFile,"rb");
    fread(&user, sizeof(struct User), 1, fp);
    fclose(fp);
    
    return user;
}

void registation(){
    printf("\n****** REGISTRATION ******");
    
    char n[100], e[50], p[30];
    
    printf("\n\nEnter your name : ");
    scanf("%s", n);
    printf("\nEnter Email : ");
    scanf("%s", e);
    printf("\nEnter Password : ");
    scanf("%s", p);
    
    struct User user;
    strcpy(user.name, n);
    strcpy(user.email, e);
    strcpy(user.password, p);
    user.isLogin = 1;
    user.balance = 0.0;
    saveUser(user);
}

int doLogin(){
    printf("\n****** LOGIN ******");
    
    char  e[50], p[30];
    
    printf("\nEnter Email : ");
    scanf("%s", e);
    printf("\nEnter Password : ");
    scanf("%s", p);
    
    
    struct User user = getUser();
    
    if(strcmp(e, user.email)==0 && strcmp(p, user.password)==0){
        printf("\nSuccesfully Logged in...");
        user.isLogin = 1;
        saveUser(user);
        return 1;
    }
    
    return 0;
}

int isLoggedIn(){
    struct User user = getUser();
    return user.isLogin;
}

void doRecursiveLogin(){
    
    if(isLoggedIn()==1){
        return;
    }
    
    printf("\nPlease Login...");
    
    int status = doLogin();
    
    for(;;){
        if(status==0){
            printf("\nLogin failed. Try Again (1) or Signup (0) ... ");
            int op;
            scanf("%d", &op);
            
            if(op==1){
                status = doLogin();
            }
            else{
                registation();
                status = 1;
            }
            
        }
        
        if(status==1){
            break;
        }
    }
    
}

void userLogout(void){
    struct User user = getUser();
    user.isLogin = 0;
    saveUser(user);
    doRecursiveLogin();
}

void showAccount(void){
    struct User user = getUser();
    printf("\nACCOUNT SUMMARY");
    printf("\n\nName : %s", user.name);
    printf("\nEmail : %s", user.email);
    printf("\nBalance : %0.2f Tk.", user.balance);
    
}


void saveProduct(struct Product p){
    FILE *fp = fopen(ProductFile,"ab");
    fwrite(&p, sizeof(struct Product), 1, fp);
    fclose(fp);
}

void saveProducts(struct Product *p, int items){
    FILE *fp = fopen(ProductFile,"wb");
    fwrite(p, sizeof(struct Product), items, fp);
    fclose(fp);
}

void showProducts(void){
    if(isFileEmpty(ProductFile)==1){
        printf("\nNo Product to show");
        return;
    }
    
    
    printf("\n\nID\tName\tPrice\tQuantiy");
    printf("\n_____________________________");
    
    struct Product products[1000];
    
    FILE *fp = fopen(ProductFile,"rb");
    
    int n = 0;
    for (n=0; !feof(fp); ++n)
    {
        if ( fread(&products[n], sizeof(struct Product), 1 ,fp) != 1){
            break;
        }
        else{
            printf("\n%d. %s\t%0.2lf\t%d", n+1, products[n].name, products[n].price, products[n].quantity);
        }
    }
    
    fclose(fp);
}

void productEntry(void){
    printf("\n\n (1) Add  (2) Restore");
    printf("\n\n Choose : ");
    
    int op;
    scanf("%d", &op);
    
    if(op==1){
        productAdd();
    }
    else{
        productRestore();
    }
    
}

void productAdd(void){
    
    char n[100];
    int op;

    struct Product product;
    
    printf("\nEnter Product name : ");
    scanf("%s", n);
    printf("\nEnter Price : ");
    scanf("%lf", &product.price);
    printf("\nEnter Quantity : ");
    scanf("%d", &product.quantity);
    
    strcpy(product.name, n);
    saveProduct(product);
    
    printf("\nAdd More (1/0)? : ");
    scanf("%d", &op);
    
    if(op==1){
        productAdd();
    }
    
}


void productRestore(void){
    
    if(isFileEmpty(ProductFile)==1){
        printf("\nNo Product to show");
        return;
    }
    
    
    printf("\n\nID\tName\tPrice\tQuantiy");
    printf("\n__________________________");
    
    struct Product products[1000];
    
    FILE *fp = fopen(ProductFile,"rb");
    
    int n = 0;
    for (n=0; !feof(fp); ++n)
    {
        if ( fread(&products[n], sizeof(struct Product), 1 ,fp) != 1){
            break;
        }
        else{
            printf("\n%d. %s\t%0.lf\t%d", n+1, products[n].name, products[n].price, products[n].quantity);
        }
    }
    
    fclose(fp);
    
    
    int id, quantity, itemCount=0;
    
    
    int op;
    
    do{
        printf("\n\nEnter Product ID :");
        scanf("%d", &id);
        
        if(id<1 || id>n){
            printf("\nERROR: Invalid ID!");
            printf("\nTry Again (1/0)? :");
            scanf("%d", &op);
            
            if(op==0){
                break;
            }
            else{
                continue;
            }
        }
        
        
        printf("\nEnter Restore Quantity :");
        scanf("%d", &quantity);
        
        if(quantity<1){
            printf("\nERROR: Enter valid quantity!");
            printf("\nTry Again (1/0)? :");
            
            scanf("%d", &op);
            
            if(op==0){
                break;
            }
            else{
                continue;
            }
        }
        
        
        
        
        products[id-1].quantity = products[id-1].quantity + quantity;
        
        itemCount++;
        
        printf("\nAdd more (1/0)? : ");
        scanf("%d", &op);
        
        if(op==0){
            break;
        }
        
    } while(1);
    
    if(itemCount>0){
        saveProducts(products, n);
    }
}


void saveTransaction(struct Transaction p){
    FILE *fp = fopen(TransactionFile,"ab");
    fwrite(&p, sizeof(struct Transaction), 1, fp);
    fclose(fp);
}

void saveTransactions(struct Transaction *p, int items){
    FILE *fp = fopen(TransactionFile,"wb");
    fwrite(p, sizeof(struct Transaction), items, fp);
    fclose(fp);
}


void showTransactions(void){
    
    if(isFileEmpty(TransactionFile)==1){
        printf("\nNo Transaction to show");
        return;
    }
    
    struct Transaction transactions[1000];
    
    FILE *fp = fopen(TransactionFile,"rb");
    
    int n = 0;
    for (n=0; !feof(fp); ++n)
    {
        if ( fread(&transactions[n],sizeof(struct Transaction),1 ,fp) != 1){
            break;
        }
        else{
            printf("\n\nTRANSACTION : (%d)", n+1);
            printf("\n_________________________________________________");
            
            printf("\n\nSummary : No of Products - %d, Total Price - %0.1lf", transactions[n].quantity, transactions[n].price);
            printf("\n\nDetails : \n%s", transactions[n].details);
        }
    }
    
    fclose(fp);
    
}

void sellProduct(void){
    
    if(isFileEmpty(ProductFile)==1){
        printf("\nNo Product to show");
        return;
    }
    
    
    printf("\n\nID\tName\tPrice\tQuantiy");
    printf("\n__________________________");
    
    struct Product products[1000];
    
    FILE *fp = fopen(ProductFile,"rb");
    
    int n = 0;
    for (n=0; !feof(fp); ++n)
    {
        if ( fread(&products[n], sizeof(struct Product), 1 ,fp) != 1){
            break;
        }
        else{
            printf("\n%d. %s\t%0.lf\t%d", n+1, products[n].name, products[n].price, products[n].quantity);
        }
    }
    
    fclose(fp);
    
    struct Transaction trans;
    trans.quantity = 0;
    trans.price = 0.0;
    trans.details[0] = '\0';
    
    int id, quantity, itemCount=0;
    
    
    int op;
    
    do{
        printf("\n\nEnter Product ID :");
        scanf("%d", &id);
        
        if(id<1 || id>n){
            printf("\nERROR: Invalid ID!");
            printf("\nTry Again (1/0)? :");
            scanf("%d", &op);
            
            if(op==0){
                break;
            }
            else{
                continue;
            }
        }
        
        struct Product selProduct = products[id-1];

        if(selProduct.quantity==0){
            printf("\nERROR: Restore the Products soon!");
            continue;
        }
        
        printf("\nEnter Quantity :");
        scanf("%d", &quantity);
        
        if(quantity>selProduct.quantity){
            printf("\nERROR: Enter valid quantity!");
            printf("\nTry Again (1/0)? :");
            
            scanf("%d", &op);
            
            if(op==0){
                break;
            }
            else{
                continue;
            }
        }
        
        
        trans.quantity = trans.quantity + quantity;
        trans.price = trans.price + ((double)quantity) * selProduct.price;
        
        char qString[50];
        
        sprintf(qString, "%d.", itemCount+1);
        strcat(trans.details,qString);
        strcat(trans.details,"\t");
        
        strcat(trans.details,selProduct.name);
        strcat(trans.details,"\t");
        
        sprintf(qString, "%d", quantity);
        strcat(trans.details,qString);
        
        sprintf(qString, "%0.2lf", ((double)quantity) * selProduct.price);
        strcat(trans.details,"\t");
        strcat(trans.details,qString);
        
        strcat(trans.details,"\n");
        
        products[id-1].quantity = products[id-1].quantity - quantity;
        
        itemCount++;
        
        printf("\nAdd more (1/0)? : ");
        scanf("%d", &op);
        
        if(op==0){
            break;
        }
        
    } while(1);
    
    if(itemCount>0){
        saveTransaction(trans);
        
        struct User user = getUser();
        user.balance = user.balance + trans.price;
        saveUser(user);
        
        saveProducts(products, n);
        
        showReceipt(trans);
    }
}



void showReceipt(struct Transaction trans){
    
    printf("\n\nReceipt is being print .......");

    printf("\n\n\nMahfuj Super Shop Ltd.");
    printf("\n===============================================");
    
    printf("\n\nSL\tProduct\tQty\tPrice");
    printf("\n_______________________________________________");

    printf("\n\n%s", trans.details);
    
    printf("\n_______________________________________________");
    
    printf("\n\nNo of Products = %d\t\tTotal Price = %0.2f\n\n", trans.quantity, trans.price);
    
}



void showMenuList(void){
    
    printf("\n\n (1) Sell Products");
    printf("\n (2) Product Entry");
    printf("\n (3) Product List");
    printf("\n (4) Account");
    printf("\n (5) Transaction History");
    printf("\n (6) Logout");
    printf("\n (0) Exit");
    printf("\n\n Choose : ");
    int op;
    scanf("%d", &op);
    
    switch (op) {
        case 1: {
            sellProduct();
            break;
        }
            
        case 2:
        {
            printf("\n PRODUCT ENTRY :: ");
            productEntry();
            break;
        }
            
        case 3:
        {
            printf("\n PRODUCT LIST :: ");
            showProducts();
            break;
        }
            
        case 4:{
            showAccount();
            break;
        }
            
        case 5: {
            showTransactions();
            break;
        }
            
        case 6: {
            userLogout();
            break;
        }
            
        case 0:{
            printf("\n SHOP IS BEING CLOSED.\n");
            exit(0);
            break;
        }
            
        default: break;
    }
    
    printf("\n\nContinue menu (1/0)? : ");
    scanf("%d", &op);
    
    if(op==1){
        showMenuList();
    }
}





int main(int argc, const char * argv[]) {
    
    checkOrCreateFiles();
    
    if(isFileEmpty(UserFile)==1){
        printf("\nPlease do registration first...!\n");
        registation();
    }
    else{
        doRecursiveLogin();
    }
    
    printf("\n\n****** MENU ******");
    showMenuList();
    
    return 0;
}



