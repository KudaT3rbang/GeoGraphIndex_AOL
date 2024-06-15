#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>

#define M_PI 3.14159265358979323846
#define red   "\x1B[31m"
#define grn   "\x1B[32m"
#define yel   "\x1B[33m"
#define blu   "\x1B[34m"
#define mag   "\x1B[35m"
#define cyn   "\x1B[36m"
#define wht   "\x1B[37m"
#define reset "\x1B[0m"

/* Struct Declaration */
struct nodeCity {
    char cityName[30], cityDescription[1000];
    double cityLat, cityLon;
    int continentIndex, height;
    struct nodeCity *left;
    struct nodeCity *right;
};

struct nodeContinent {
    char cityName[30], cityDescription[1000];
    double cityLat, cityLon;
    struct nodeContinent *next;
};

struct nodeCity *rootByCity = NULL;
struct nodeCity *rootByCoor = NULL;
struct nodeCity *recentlySearch = NULL;

struct nodeContinent *continentList[7] = { 0 };

int NumberOfCities=0; //untuk track total city
int prefixCount=0;

/* Function Declaration */
// [1] Function For AVL Tree
int max(int left, int right);
int getHeight(struct nodeCity *root);
int getBFactor(struct nodeCity *root);
struct nodeCity *rightRotate(struct nodeCity *root);
struct nodeCity *leftRotate(struct nodeCity *root);
struct nodeCity *newCityNode(double lat, double lon, const char *newCity, const char *cityDescription, int continentIndex);
struct nodeCity *insertByCoor(struct nodeCity *root, double lat, double lon,
                              const char *cityName, const char *cityDescription, int continentIndex);
struct nodeCity *insertByCityName(struct nodeCity *root,
                                  struct nodeCity **rootCoor, double lat,
                                  double lon, const char *cityName, const char *cityDescription, int continentIndex);
void insertMenu();

// [2] Function For Sorting and Printing AVL Tree
const char *getContinentName(int index);
void displaySortingOptions();
void printTableHeader();
void sortByCityName(struct nodeCity *root);
void sortByCoordinates(struct nodeCity *root);
void printCityNode(struct nodeCity *node);
void preorder_viewAllCity(struct nodeCity *root);
void postorder_viewAllCity(struct nodeCity *root);
void inorder_viewAllCity(struct nodeCity *root);
void displayPage(struct nodeCity** cities, int startIndex, int totalCities) ;
void inOrderTraversal(struct nodeCity* root, struct nodeCity** cities, int* index);
void displayPageDescending(struct nodeCity** cities, int startIndex, int totalCities);
void sortMenu();

// [3] Search Function
struct nodeCity *search(struct nodeCity *root, char *cityName);
struct nodeCity *searchCoor(struct nodeCity *root, double lat, double lon);
void recentSearch();
void displayCityDetails(struct nodeCity* city);
struct nodeCity *searchCityMenu();
void searchCoorMenu();
void searchMenu(struct nodeCity *root);

// [4] Delete Function
struct nodeCity *minValueNode(struct nodeCity *node);
struct nodeCity *deleteNodeByCity(struct nodeCity *root, char *cityName);
struct nodeCity *deleteNodeByCoor(struct nodeCity *root, double lat, double lon);
void deleteMenu();

// [5] Function For Array Of Linked List (Continent)
int pushNodeContinent(struct nodeContinent **head, const char *cityName, const char *cityDescription, double lat, double lon);
void printContinent(struct nodeContinent *continentList[]);

// [6] Utility Function
void utilPressAnyKey();
int utilValidateTitle(const char *name);
void utilTrimTrailingSpaces(char *str);

// [7] Open/Save Data Function
struct nodeCity *readAvlFromFile(FILE *fp, struct nodeCity *root);
void writeAvlToFile(struct nodeCity *root, FILE *fp);
void openData();
void saveDataCity();
void saveDataCoor();

// [8] Distance Checking Function
double deg2rad(double deg);
double distanceCheck(double lat1, double lon1, double lat2, double lon2);
void searchDistanceBetweenCities(struct nodeCity *root);

// [9] Update City Description
void updateCityDescription();

/* Main Function */
int main(void) {
    openData();
    do {
        int choice;
        printf("Welcome to GeoGraphIndex Program\n");
        printf("-----------------------------------\n");
        printf("1. Add a city\n");
        printf("2. View all cities\n");
        printf("3. Find a city\n");
        printf("4. Delete a city\n");
        printf("5. Find distance of two cities\n");
        printf("6. View cities based on continent\n");
        printf("7. Update City Description\n");
        printf("0. Exit\n");
        printf("-----------------------------------\n");
        printf("Input >> ");
        do {
            scanf("%d", &choice);
            getchar();
        } while(choice < 0 || choice > 7);
        switch(choice) {
            case 1:
                insertMenu();
                utilPressAnyKey();
                break;
            case 2:
                sortMenu();
                break;
            case 3:
                searchMenu(rootByCity);
                break;
            case 4:
                deleteMenu();
                break;
            case 5:
                searchDistanceBetweenCities(rootByCity);
                break;
            case 6:
                printContinent(continentList);
                break;
            case 7:
                updateCityDescription();
                break;
            case 0:
                saveDataCity();
                saveDataCoor();
                return 0;
                break;
            default:
                printf("Invalid input!\n");
        }
    } while(1);
}

/* Function Definition */
// [1] Function For AVL Tree
int max(int left, int right) {
    // Returning max value from two parameter
    return (right > left) ? right : left;
};

int getHeight(struct nodeCity *root) {
    // Returning root height, if root is NULL then return 0
    return (root == NULL) ? 0 : root->height;
};

int getBFactor(struct nodeCity *root) {
    // Returning B Factor of node, if root is NULL then return 0
    return (root == NULL) ? 0 : getHeight(root->left) - getHeight(root->right);
};

struct nodeCity *rightRotate(struct nodeCity *root) {
    struct nodeCity *newRoot = root->left; // Set newRoot node to left child of current root
    struct nodeCity *temp = newRoot->right;

    // Perform rotation
    newRoot->right = root;
    root->left = temp;

    // Update node height
    root->height = max(getHeight(root->left), getHeight(root->right)) + 1;
    newRoot->height = max(getHeight(newRoot->left), getHeight(newRoot->right)) + 1;

    return newRoot;
};

struct nodeCity *leftRotate(struct nodeCity *root) {
    struct nodeCity *newRoot = root->right; // Set newRoot node to right child of current root
    struct nodeCity *temp = newRoot->left;

    // Perform rotation
    newRoot->left = root;
    root->right = temp;

    // Update node height
    root->height = max(getHeight(root->left), getHeight(root->right)) + 1;
    newRoot->height = max(getHeight(newRoot->left), getHeight(newRoot->right)) + 1;

    return newRoot;
};

struct nodeCity *newCityNode(double lat, double lon, const char *newCity, const char *cityDescription, int continentIndex) {
    struct nodeCity *current = (struct nodeCity *)malloc(sizeof(struct nodeCity));
    strcpy(current->cityName, newCity);
    strcpy(current->cityDescription, cityDescription);
    current->cityLat = lat;
    current->cityLon = lon;
    current->continentIndex = continentIndex;
    current->height = 1;
    current->left = current->right = NULL;
    return current;
};

struct nodeCity *insertByCoor(struct nodeCity *root, double lat, double lon, const char *cityName, const char *cityDescription, int continentIndex) {
    if (root == NULL) {
        return newCityNode(lat, lon, cityName, cityDescription, continentIndex);
    }

    if (lat < root->cityLat || (lat == root->cityLat && lon < root->cityLon)) {
        // Kalau latitude lebih kecil dari latitude current node, atau jika latitude
        // sama tapi longitude lebih kecil dari current node longitude, buat node ke
        // subtree kiri
        root->left = insertByCoor(root->left, lat, lon, cityName, cityDescription, continentIndex);
    } else if (lat > root->cityLat ||
               (lat == root->cityLat && lon > root->cityLon)) {
        // Kalau latitude lebih besar dari latitude current node, atau jika latitude
        // sama tapi longitude lebih besar dari current node longitude, buat node ke
        // subtree kanan
        root->right = insertByCoor(root->right, lat, lon, cityName, cityDescription, continentIndex);
    } else {
        // Kalau latitude sama dan longitude sama, maka cityName yang baru akan
        // dikebalikan tanpa membuat node baru
        return root;
    }

    root->height = max(getHeight(root->left), getHeight(root->right)) + 1;
    int balance = getBFactor(root);

    // LL Case
    if (balance > 1 &&
        (lat < root->left->cityLat ||
         (lat == root->left->cityLat && lon < root->left->cityLon))) {
        return rightRotate(root);
    }

    // RR Case
    if (balance < -1 &&
        (lat > root->right->cityLat ||
         (lat == root->right->cityLat && lon > root->right->cityLon))) {
        return leftRotate(root);
    }

    // LR Case
    if (balance > 1 &&
        (lat > root->left->cityLat ||
         (lat == root->left->cityLat && lon > root->left->cityLon))) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // RL Case
    if (balance < -1 &&
        (lat < root->right->cityLat ||
         (lat == root->right->cityLat && lon < root->right->cityLon))) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
};

struct nodeCity *insertByCityName(struct nodeCity *root,
                                  struct nodeCity **rootCoor, double lat,
                                  double lon, const char *cityName, const char *cityDescription, int continentIndex) {
    /* Insert a new city into the tree */
    if (root == NULL) {
        root = newCityNode(lat, lon, cityName, cityDescription, continentIndex);
        pushNodeContinent(&continentList[continentIndex - 1], cityName, cityDescription, lat, lon);
        // Buat node untuk AVL tree City sekaligus buat node untuk AVL tree Coor
        *rootCoor = insertByCoor(*rootCoor, lat, lon, cityName, cityDescription, continentIndex);
    } else if (strcmp(cityName, root->cityName) < 0) {
        root->left = insertByCityName(root->left, rootCoor, lat, lon,
                                      cityName, cityDescription, continentIndex); // Insert to the left subtree
    } else {
        root->right = insertByCityName(root->right, rootCoor, lat, lon,
                                       cityName, cityDescription, continentIndex); // Insert to the right subtree
    }

    root->height = max(getHeight(root->left), getHeight(root->right)) + 1;
    int bFactor = getBFactor(root);

    // LL Case
    if (bFactor > 1 && strcmp(cityName, root->left->cityName) < 0) {
        return rightRotate(root);
    }
    // RR Case
    if (bFactor < -1 && strcmp(cityName, root->right->cityName) > 0) {
        return leftRotate(root);
    }
    // LR Case
    if (bFactor > 1 && strcmp(cityName, root->left->cityName) > 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    // RL Case
    if (bFactor < -1 && strcmp(cityName, root->right->cityName) < 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    return root;
};

void insertMenu() {
    system("cls");
    printf("===== Add a city =====\n\n");
    char cityName[30], cityDescription[1000];
    double lat, lon;
    int continentIndex;
    struct nodeCity *cityNode;
    struct nodeCity *coorNode;

    do {
        printf("Input City Name [5-30]: ");
        scanf("%30[^\n]%*c", cityName);
        cityNode = search(rootByCity, cityName);
        if (utilValidateTitle(cityName)) {
            printf("City Name must be between 5 and 30 characters!\n");
        } else if (cityNode != NULL) {
            printf("City Name already exists!\n");
        }
    } while (utilValidateTitle(cityName) || cityNode != NULL);

    printf("Input City Description: ");
    scanf("%1000[^\n]%*c", cityDescription);

    do {
        printf("Input City Latitude: ");
        scanf("%lf", &lat);
        getchar();

        printf("Input City Longitude: ");
        scanf("%lf", &lon);
        getchar();

        coorNode = searchCoor(rootByCoor, lat, lon);
        if (coorNode != NULL) {
            printf("Coordinate already exists!\n");
        }
    } while (coorNode != NULL);


    printf("Input Continent :\n");
    printf("1. North America\n");
    printf("2. South America\n");
    printf("3. Europe\n");
    printf("4. Africa\n");
    printf("5. Asia\n");
    printf("6. Australia\n");
    printf("7. Antartica\n");
    printf("Input >> ");
    do {
        scanf("%d", &continentIndex);
        getchar();
    } while (continentIndex < 0 || continentIndex > 5);

    rootByCity = insertByCityName(rootByCity, &rootByCoor, lat, lon, cityName, cityDescription, continentIndex);
    NumberOfCities++;
    printf("City added successfully!\n");
};

// [2] Function For Sorting and Printing AVL Tree
const char *getContinentName(int index) {
    switch (index) {
        case 1:
            return "North America";
        case 2:
            return "South America";
        case 3:
            return "Europe";
        case 4:
            return "Africa";
        case 5:
            return "Asia";
        case 6:
            return "Australia";
        case 7:
            return "Antarctica";
        default:
            return "Unknown";
    }
}

void displaySortingOptions() {
    printf("View in which order?\n1. Preorder Traversal\n2. Inorder Traversal\n3. Postorder Traversal\nInput>> ");
}

void printTableHeader() {
    for (int i = 0; i < 126; i++) printf("-");
    printf("\n");
    printf("|%-30s|%-30s|%-20s|%-20s|%-20s|\n", "City Name", "City Description", "Latitude", "Longitude", "Continent");
    for (int i = 0; i < 126; i++) printf("-");
    printf("\n");
}

void sortByCityName(struct nodeCity *root) {
    printf("======= Sort by city name =======\n\n");
    displaySortingOptions();

    int choice;
    do {
        scanf("%d", &choice);
        getchar();
    } while (choice < 1 || choice > 3);

    printTableHeader();

    if (choice == 1)
        preorder_viewAllCity(root);
    else if (choice == 2)
        inorder_viewAllCity(root);
    else if (choice == 3)
        postorder_viewAllCity(root);
};

void sortByCoordinates(struct nodeCity *root) {
    printf("========== Sort based on coordinates ==========\n");
    displaySortingOptions();

    int choice;
    do {
        scanf("%d", &choice);
        getchar();
    } while (choice < 1 || choice > 3);

    printTableHeader();

    if (choice == 1)
        preorder_viewAllCity(root);
    else if (choice == 2)
        inorder_viewAllCity(root);
    else if (choice == 3)
        postorder_viewAllCity(root);
};

void printCityNode(struct nodeCity *node) {
    printf("|%-30s|%-30s|%-20.6f|%-20.6f|%-20s|\n", node->cityName, node->cityDescription, node->cityLat, node->cityLon, getContinentName(node->continentIndex));
    for (int i = 0; i < 126; i++) {
        printf("-");
    }
    printf("\n");
}

void preorder_viewAllCity(struct nodeCity *root) {
    if (root == NULL) {
        return;
    }
    printCityNode(root);
    preorder_viewAllCity(root->left);
    preorder_viewAllCity(root->right);
}

void postorder_viewAllCity(struct nodeCity *root) {
    if (root == NULL) {
        return;
    }
    postorder_viewAllCity(root->left);
    postorder_viewAllCity(root->right);
    printCityNode(root);
}

void inorder_viewAllCity(struct nodeCity *root) {
    if (root == NULL) {
        return;
    }
    inorder_viewAllCity(root->left);
    printCityNode(root);
    inorder_viewAllCity(root->right);
}

void displayPage(struct nodeCity** cities, int startIndex, int totalCities) {
    system("cls"); // Clear the screen
    printTableHeader();
    int endIndex = startIndex + 20 < totalCities ? startIndex + 20 : totalCities;
    for (int i = startIndex; i < endIndex; i++) {
        if(strlen(cities[i]->cityDescription)<=30)
            printf("|%-30s|%-30s|%-20.6f|%-20.6f|%-20s|\n", cities[i]->cityName, cities[i]->cityDescription, cities[i]->cityLat, cities[i]->cityLon, getContinentName(cities[i]->continentIndex));
        else
            printf("|%-30s|%-26.26s....|%-20.6f|%-20.6f|%-20s|\n", cities[i]->cityName, cities[i]->cityDescription, cities[i]->cityLat, cities[i]->cityLon, getContinentName(cities[i]->continentIndex));


        for (int j = 0; j < 126; j++) printf("-"); // Changed variable i to j
        printf("\n");
    }
    printf("Displaying %d - %d out of %d\n", startIndex + 1, endIndex, totalCities);
    printf("\n\n [W / ^] Previous  [S / v] Next  [enter] Back to main menu\n");
}

void displayPageDescending(struct nodeCity** cities, int startIndex, int totalCities) {
    system("cls"); // Clear the screen
    printTableHeader();
    int endIndex = startIndex + 20 < totalCities ? startIndex + 20 : totalCities;
    // Calculate the starting and ending indices for the loop in descending order
    int startDescIndex = totalCities - 1 - startIndex;
    int endDescIndex = startDescIndex - (endIndex - startIndex);

    for (int i = startDescIndex; i > endDescIndex; i--) {
        printf("|%-30s|%-30s|%-20.6f|%-20.6f|%-20s|\n", cities[i]->cityName, cities[i]->cityDescription, cities[i]->cityLat, cities[i]->cityLon, getContinentName(cities[i]->continentIndex));
        for (int j = 0; j < 126; j++) printf("-");
        printf("\n");
    }
    // Adjust the displayed range of cities to match the descending order
    int displayedStart = totalCities - startDescIndex;
    int displayedEnd = totalCities - endDescIndex - 1;
    printf("Displaying %d - %d out of %d\n", displayedStart, displayedEnd, totalCities);
    printf("\n\n [^] Previous  [v] Next  [enter] Back to main menu\n");
}

void inOrderTraversal(struct nodeCity* root, struct nodeCity** cities, int* index) {
    if (root != NULL) {
        inOrderTraversal(root->left, cities, index);
        cities[*index] = root;
        (*index)++;
        inOrderTraversal(root->right, cities, index);
    }
}

void sortMenu() {
    system("cls");
    int n;
    if(rootByCity == NULL) {
        printf("There is no data!\n");
        utilPressAnyKey();
        return;
    }

    struct nodeCity* cities[NumberOfCities];
    int index = 0;

    printf("======== View and sort all cities ========\n\n" );
    printf("1. Sort based on city name\n2. Sort based on coordinates\n\nInput>> ");
    scanf("%d", &n);
    getchar();
    if (n == 1) {
        inOrderTraversal(rootByCity, cities, &index);
        printf("1. Ascending\n2. Descending\n\nInput>> ");
        scanf("%d", &n);

        if (n==1){
            displayPage(cities, 0, NumberOfCities);
        }
        else if (n==2){
            displayPageDescending(cities, 0, NumberOfCities);
        }
        else {
            printf("Invalid choice!\n");
            utilPressAnyKey();
            return;
        }
    }
    else if (n == 2) {
        inOrderTraversal(rootByCoor, cities, &index);
        printf("1. Ascending\n2. Descending\n\nInput>> ");
        scanf("%d", &n);
        if (n==1){
            displayPage(cities, 0, NumberOfCities);
        }
        else if (n==2){
            displayPageDescending(cities, 0, NumberOfCities);
        }
        else{
            printf("Invalid choice!\n");
            utilPressAnyKey();
            return;
        }

    } else {
        printf("Invalid choice!\n");
        utilPressAnyKey();
        return;
    }


    int currentPage = 0;
    int maxPage = NumberOfCities / 20;
    if(NumberOfCities % 20 == 0)
        maxPage--;

    char select; // Variable to store the current selection

    do
    {
        displayPage(cities, currentPage * 20, NumberOfCities);
        select = getch();

        if(select == 72 || select == 'W' || select == 'w')
        {
            currentPage--;
            if(currentPage < 0)
                currentPage = 0;
        }
        else if(select == 80 || select == 'S' || select == 's')
        {
            currentPage++;
            if(currentPage > maxPage)
                currentPage = maxPage;
        }
    } while(select != '\r');

    utilPressAnyKey();
}

// [3] Search Function
struct nodeCity *search(struct nodeCity *root, char *cityName) {
    if (root == NULL || strcasecmp(root->cityName, cityName) == 0) return root;

    if (strcasecmp(root->cityName, cityName) < 0) return search(root->right, cityName);

    return search(root->left, cityName);
};

struct nodeCity *searchCoor(struct nodeCity *root, double lat, double lon) {
    if (root == NULL || (lat == root->cityLat && lon == root->cityLon)) return root;

    if (lat > root->cityLat || (lat == root->cityLat && lon > root->cityLon)) {
        return searchCoor(root->right, lat, lon);
    } else {
        return searchCoor(root->left, lat, lon);
    }
};

void prefix(struct nodeCity *root, char pref[]) {
    if (root != NULL) {
        prefix(root->left, pref);
        // Convert city name and prefix to lowercase for case-insensitive comparison
        char cityNameLower[26];
        char prefixLower[26];
        strcpy(cityNameLower, root->cityName);
        strcpy(prefixLower, pref);

        for (int i = 0; cityNameLower[i] && i < 25; i++) {
            cityNameLower[i] = tolower(cityNameLower[i]);
        }
        for (int i = 0; prefixLower[i] && i < 25; i++) {
            prefixLower[i] = tolower(prefixLower[i]);
        }

        // Check if prefix matches
        if (strstr(cityNameLower, prefixLower) != NULL) {
            if (prefixCount++ == 0) {
                printTableHeader();
            }
            // Print city information with formatting based on description length
            if (strlen(root->cityDescription) <= 30)
                printf("|%-30s|%-30s |%-20.6f |%-20.6f |%-20s|\n", root->cityName, root->cityDescription, root->cityLat, root->cityLon, getContinentName(root->continentIndex));
            else
                printf("|%-30s|%-26.26s....|%-20.6f| %-20.6f|%-20s|\n", root->cityName, root->cityDescription, root->cityLat, root->cityLon, getContinentName(root->continentIndex));

            printf("------------------------------------------------------------------------------------------------------------------------------\n");
        }
        prefix(root->right, pref);
    }
}

void recentSearch() {
    if(recentlySearch == NULL) {
        return;
    } else {
        printf("Recent Search %s (%f, %f)\n", recentlySearch->cityName, recentlySearch->cityLat, recentlySearch->cityLon);
    }
};

void displayCityDetails(struct nodeCity* city) {
    recentlySearch = city;
    if (city == NULL) {
        printf("City not found!\n");
    } else {
        printf("City Found!\n");
        printf("===========================\n");
        printf("City Name : %s\n", city->cityName);
        printf("City Description :\n");
        printf("%s\n", city->cityDescription);
        printf("Latitude : %lf\n", city->cityLat);
        printf("Longitude : %lf\n", city->cityLon);
        printf("Continent : %s\n", getContinentName(city->continentIndex));
        printf("===========================\n");
    }
}

struct nodeCity *searchCityMenu() {
    char pref[100] = "";
    char temp = '\0';
    prefixCount = 0;
    do {
        system("cls");
        printf("%s=======%s view worker %s=======\n\n%s", blu, reset, blu, reset);
        printf(yel);
        recentSearch();
        printf("Input a prefix to be searched: %s\n", pref);
        printf(reset);

        prefixCount = 0;
        prefix(rootByCity, pref);

        temp = getch();
        if (temp == '\b') {
            if (strlen(pref) > 0) {
                pref[strlen(pref) - 1] = '\0';
            }
        } else {
            sprintf(pref, "%s%c", pref, temp);
        }
    } while (temp != '\r');

    utilTrimTrailingSpaces(pref);
    system("cls");
    struct nodeCity* curr;
    if (strlen(pref) > 0 && prefixCount == 1) {
        curr = search(rootByCity, pref);
        displayCityDetails(curr);
        return curr;
    } else {
        printf("City not found!\n");
        return NULL;
    }
}

void searchCoorMenu() {
    double lat, lon;
    recentSearch();
    printf("Enter Latitude: ");
    scanf("%lf", &lat);
    getchar();
    printf("Enter Longitude: ");
    scanf("%lf", &lon);
    getchar();

    struct nodeCity *curr = searchCoor(rootByCoor, lat, lon);
    if (curr == NULL) {
        printf("City not found!\n");
    } else {
        displayCityDetails(curr);
    }
}

void searchMenu(struct nodeCity *root) {

    system("cls");
    int choice;
    char key[30];
    double lat, lon;
    printf("===== Find a city =====\n\n");
    printf("Search based on?\n1. City name\n2. Coordinates\n\nInput>> ");
    do {
        scanf("%d", &choice);
        getchar();
    } while (choice < 1 || choice > 2);

    switch (choice) {
        case 1: {
            searchCityMenu();
            break;
        }
        case 2: {
            searchCoorMenu();
            break;
        }
        default:
            printf("Invalid choice!\n");
    }
    utilPressAnyKey();
}


// [4] Delete Function
struct nodeCity *minValueNode(struct nodeCity *node) {
    struct nodeCity *current = node;
    while (current->left != NULL) {
        current = current->left;
    }
    return current;
}

struct nodeCity *deleteNodeByCity(struct nodeCity *root, char *cityName) {
    if (root == NULL) return root;

    if (strcasecmp(cityName, root->cityName) < 0) {
        root->left = deleteNodeByCity(root->left, cityName);
    } else if (strcasecmp(cityName, root->cityName) > 0) {
        root->right = deleteNodeByCity(root->right, cityName);
    } else {
        if ((root->left == NULL) || (root->right == NULL)) {
            struct nodeCity *temp = root->left ? root->left : root->right;
            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else {
                *root = *temp;
            }
            free(temp);
        } else {
            struct nodeCity *temp = minValueNode(root->right);
            strcpy(root->cityName, temp->cityName);
            strcpy(root->cityDescription, temp->cityDescription);
            root->cityLat = temp->cityLat;
            root->cityLon = temp->cityLon;
            root->continentIndex = temp->continentIndex;
            root->right = deleteNodeByCity(root->right, temp->cityName);
        }
    }

    if (root == NULL) return root;

    root->height = max(getHeight(root->left), getHeight(root->right)) + 1;
    int balance = getBFactor(root);

    // LL Case
    if (balance > 1 && getBFactor(root->left) >= 0) {
        return rightRotate(root);
    }

    // LR Case
    if (balance > 1 && getBFactor(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // RR Case
    if (balance < -1 && getBFactor(root->right) <= 0) {
        return leftRotate(root);
    }

    // RL Case
    if (balance < -1 && getBFactor(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

struct nodeCity *deleteNodeByCoor(struct nodeCity *root, double lat, double lon) {
    if (root == NULL) return root;

    if (lat < root->cityLat || (lat == root->cityLat && lon < root->cityLon)) {
        root->left = deleteNodeByCoor(root->left, lat, lon);
    } else if (lat > root->cityLat || (lat == root->cityLat && lon > root->cityLon)) {
        root->right = deleteNodeByCoor(root->right, lat, lon);
    } else {
        if ((root->left == NULL) || (root->right == NULL)) {
            struct nodeCity *temp = root->left ? root->left : root->right;
            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else {
                *root = *temp;
            }
            free(temp);
        } else {
            struct nodeCity *temp = minValueNode(root->right);
            root->cityLat = temp->cityLat;
            root->cityLon = temp->cityLon;
            strcpy(root->cityName, temp->cityName);
            strcpy(root->cityDescription, temp->cityDescription);
            root->continentIndex = temp->continentIndex;
            root->right = deleteNodeByCoor(root->right, temp->cityLat, temp->cityLon);
        }
    }

    if (root == NULL) return root;

    root->height = max(getHeight(root->left), getHeight(root->right)) + 1;
    int balance = getBFactor(root);

    // LL Case
    if (balance > 1 && getBFactor(root->left) >= 0) {
        return rightRotate(root);
    }

    // LR Case
    if (balance > 1 && getBFactor(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // RR Case
    if (balance < -1 && getBFactor(root->right) <= 0) {
        return leftRotate(root);
    }

    // RL Case
    if (balance < -1 && getBFactor(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

void deleteMenu() {
    system("cls");
    int choice;
    char cityName[30];
    double lat, lon;

    printf("===== Delete a city =====\n\n");
    printf("Delete based on?\n1. City name\n2. Coordinates\n\nInput>> ");
    do {
        scanf("%d", &choice);
        getchar();
    } while (choice < 1 || choice > 2);

    switch (choice) {
        case 1:
            printf("Enter a city name to delete: ");
            scanf("%29[^\n]", cityName);
            getchar();
            rootByCity = deleteNodeByCity(rootByCity, cityName);
            rootByCoor = deleteNodeByCoor(rootByCoor, rootByCity->cityLat, rootByCity->cityLon);
            break;
        case 2:
            printf("Enter Latitude: ");
            scanf("%lf", &lat);
            getchar();
            printf("Enter Longitude: ");
            scanf("%lf", &lon);
            getchar();
            rootByCoor = deleteNodeByCoor(rootByCoor, lat, lon);
            rootByCity = deleteNodeByCity(rootByCity, rootByCoor->cityName);
            break;
        default:
            printf("Invalid choice!\n");
    }
    NumberOfCities--;
    printf("City successfully deleted!\n");
    utilPressAnyKey();
}

// [5] Function For Array Of Linked List (Continent)
int pushNodeContinent(struct nodeContinent **head, const char *cityName, const char *cityDescription, double lat, double lon) {
    struct nodeContinent *newNode = malloc(sizeof(struct nodeContinent));
    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        return -1;
    }
    strcpy(newNode->cityName, cityName);
    strcpy(newNode->cityDescription, cityDescription);
    newNode->cityLat = lat;
    newNode->cityLon = lon;
    newNode->next = *head;
    *head = newNode;
    return 0;
};

void printContinent(struct nodeContinent *continentList[]) {
    int continentIndex;
    const char *continentNames[] = {"North America", "South America", "Europe", "Africa", "Asia", "Australia", "Antarctica"};

    printf("Select a continent to view:\n");
    for (int i = 0; i < 7; ++i) {
        printf("%d. %s\n", i + 1, continentNames[i]);
    }
    printf("Enter the index of the continent: ");
    scanf("%d", &continentIndex);
    getchar();

    if (continentIndex < 1 || continentIndex > 7) {
        printf("Invalid continent index!\n");
        return;
    }

    printf("Cities in %s:\n", continentNames[continentIndex - 1]);

    // Print table header
    for (int i = 0; i < 103; i++) printf("-");
    printf("\n");
    printf("| %-30s | %-30s | %-15s | %-15s |\n", "City Name", "City Description", "Latitude", "Longitude");
    for (int i = 0; i < 103; i++) printf("-");
    printf("\n");

    struct nodeContinent *current = continentList[continentIndex - 1];
    while (current != NULL) {
        if (strlen(current->cityDescription)<=30)
            printf("| %-30s | %-30s | %-15.6f | %-15.6f |\n", current->cityName, current->cityDescription, current->cityLat, current->cityLon);
        else
            printf("| %-30s | %-26.26s.... | %-15.6f | %-15.6f |\n", current->cityName, current->cityDescription, current->cityLat, current->cityLon);
        current = current->next;
    }

    // Print table footer
    for (int i = 0; i < 103; i++) printf("-");
    printf("\n");
    utilPressAnyKey();
};

// [6] Utility Function
void utilPressAnyKey() {
    printf("Press any key to continue...");
    getchar();
    system("cls");
};

int utilValidateTitle(const char *name) {
    return (strlen(name) < 5 || strlen(name) > 30) ? 1 : 0;
};

void utilTrimTrailingSpaces(char *str) {
    int len = strlen(str);
    while (len > 0 && isspace((unsigned char)str[len - 1])) {
        str[--len] = '\0';
    }
}

// [7] Open/Save Data Function
struct nodeCity *readAvlFromFile(FILE *fp, struct nodeCity *root) {
    double lat, lon;
    char cityName[30], cityDescription[1000];
    int continentIndex;
    while (fscanf(fp, "%lf,%lf,%29[^,],%999[^,],%d\n", &lat, &lon, cityName, cityDescription, &continentIndex) != EOF) {
        root = insertByCityName(root, &rootByCoor, lat, lon, cityName, cityDescription, continentIndex);
        NumberOfCities++;
    }
    return root;
};

void writeAvlToFile(struct nodeCity *root, FILE *fp) {
    if (root == NULL) {
        return;
    }
    fprintf(fp, "%lf,%lf,%s,%s,%d\n", root->cityLat, root->cityLon, root->cityName, root->cityDescription, root->continentIndex);
    writeAvlToFile(root->left, fp);
    writeAvlToFile(root->right, fp);
};

void openData() {
    FILE *fp = fopen("data_city.txt", "r");
    if (fp == NULL) {
        printf("No file found!\n");
        return;
    }
    rootByCity = readAvlFromFile(fp, rootByCity);
    fclose(fp);
};

void saveDataCity() {
    FILE *fp = fopen("data_coor.txt", "w");
    if (fp == NULL) {
        printf("Failed to save data!\n");
        return;
    }
    writeAvlToFile(rootByCoor, fp);
    fclose(fp);
    printf("City AVL Tree Saved\n");
};

void saveDataCoor() {
    FILE *fp = fopen("data_city.txt", "w");
    if (fp == NULL) {
        printf("Failed to save data!\n");
        return;
    }
    writeAvlToFile(rootByCity, fp);
    fclose(fp);
    printf("Coordinates AVL Tree Saved\n");
};

// [8] Distance Checking Function
double deg2rad(double deg) {
    return deg * (M_PI / 180);
}

double distanceCheck(double lat1, double lon1, double lat2, double lon2) {
    lat1 = deg2rad(lat1);
    lon1 = deg2rad(lon1);
    lat2 = deg2rad(lat2);
    lon2 = deg2rad(lon2);
    return acos(sin(lat1) * sin(lat2) +
                cos(lat1) * cos(lat2) * cos(lon2 - lon1)) *
           6371;
}

void searchDistanceBetweenCities(struct nodeCity *root) {
    char city1[35], city2[35];
    printf("====== Search the distance between two cities =====\n");
    printf("Note: the distance is in kilometres.\n\n");

    struct nodeCity *curr1, *curr2;

    do {
        printf("Enter city 1: ");
        scanf("%34[^\n]", city1);
        getchar();
        curr1 = search(rootByCity, city1);
        if (curr1 == NULL) {
            printf("City not found! Please try again.\n\n");
        }
    } while (curr1 == NULL);

    do {
        printf("Enter city 2: ");
        scanf("%34[^\n]", city2);
        getchar();
        curr2 = search(rootByCity, city2);
        if (curr2 == NULL) {
            printf("City not found! Please try again.\n\n");
        }
    } while (curr2 == NULL);

    // Calculate and display the distance
    double result = distanceCheck(curr1->cityLat, curr1->cityLon, curr2->cityLat, curr2->cityLon);
    printf("The distance between %s and %s is %.2lf kilometers.\n\n", curr1->cityName, curr2->cityName, result);
}

// [9] Update City Description
void updateCityDescription() {
    struct nodeCity* cityByName = searchCityMenu();
    if (cityByName == NULL) {
        utilPressAnyKey();
        return;
    } else {
        struct nodeCity* cityByCoor = searchCoor(rootByCoor, cityByName->cityLat, cityByName->cityLon);
        if (cityByCoor == NULL) {
            printf("City not found in coordinate-based tree!\n");
            return;
        }

        printf("\nEnter new description for %s: ", cityByName->cityName);
        char newDescription[100];
        fgets(newDescription, sizeof(newDescription), stdin);
        size_t len = strlen(newDescription);
        if (len > 0 && newDescription[len - 1] == '\n') {
            newDescription[len - 1] = '\0';
        }

        strncpy(cityByName->cityDescription, newDescription, sizeof(cityByName->cityDescription) - 1);
        cityByName->cityDescription[sizeof(cityByName->cityDescription) - 1] = '\0';

        strncpy(cityByCoor->cityDescription, newDescription, sizeof(cityByCoor->cityDescription) - 1);
        cityByCoor->cityDescription[sizeof(cityByCoor->cityDescription) - 1] = '\0';

        printf("City description updated successfully!\n");
    }
}