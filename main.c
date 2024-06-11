#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Struct Declaration */
struct nodeCity {
    char cityName[30], cityDescription[100];
    double cityLat, cityLon;
    int continentIndex, height;
    struct nodeCity *left;
    struct nodeCity *right;
};

struct nodeContinent {
    char cityName[30], cityDescription[100];
    double cityLat, cityLon;
    struct nodeContinent *next;
};

struct nodeCity *rootByCity = NULL;
struct nodeCity *rootByCoor = NULL;
struct nodeCity *recentlySearch = NULL;

struct nodeContinent *continentList[7] = { 0 };

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
void sortMenu();

// [3] Search Function
struct nodeCity *search(struct nodeCity *root, char *cityName);
struct nodeCity *searchCoor(struct nodeCity *root, double lat, double lon);
void recentSearch();
void searchMenu(struct nodeCity *root);

// [4] Delete Function
// TBA

// [5] Function For Array Of Linked List (Continent)
int pushNodeContinent(struct nodeContinent **head, const char *cityName, const char *cityDescription, double lat, double lon);
void printContinent(struct nodeContinent *continentList[]);

// [6] Utility Function
void utilPressAnyKey();
int utilValidateTitle(const char *name);

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

/* Main Function */
int main(void) {
    openData();
    do {
        int choice;
        printf("Welcome to GeoGraphIndex Program\n");
        printf("----------------------\n");
        printf("1. Add a city\n");
        printf("2. View all cities\n");
        printf("3. Find a city\n");
        printf("4. Delete a city\n");
        printf("5. Find distance of two cities\n");
        printf("6. Print Continent\n");
        printf("0. Exit\n");
        printf("----------------------\n");
        printf("Input >> ");
        do {
            scanf("%d", &choice);
            getchar();
        } while(choice < 0 || choice > 6);
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
                break;
            case 5:
                searchDistanceBetweenCities(rootByCity);
                break;
            case 6:
                printContinent(continentList);
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
    char cityName[30], cityDescription[100];
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
    scanf("%100[^\n]%*c", cityDescription);

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
    printf("City successfully added!\n");
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
    printf("=======Sort based on city name=======\n");
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
    printf("==========Sort based on coordinates==========\n");
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

void sortMenu() {
    int n;
    if(rootByCity == NULL) {
        printf("There is no data!\n");
        utilPressAnyKey();
        return;
    }

    printf("1. Sort based on city name\n2. Sort based on coordinates\n\nInput>> ");
    scanf("%d", &n);
    getchar();
    if (n == 1) {
        sortByCityName(rootByCity);
    } else if (n == 2) {
        sortByCoordinates(rootByCoor);
    } else {
        printf("Invalid choice!\n");
    }

    utilPressAnyKey();
};

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

void displayCityDetails(struct nodeCity *city) {
    printf("City details:\n");
    printf("|%-30s|%-30s|%-30s|%-10s|%-30s|\n", "City Name", "Latitude", "Longitude", "Continent", "Description");
    printf("|%-30s|%-30lf|%-30lf|%-10d|%-30s|\n", city->cityName, city->cityLat, city->cityLon, city->continentIndex, city->cityDescription);
}

void recentSearch() {
    if(recentlySearch == NULL) {
        return;
    } else {
        printf("Recent Search : %s (%f, %f)\n", recentlySearch->cityName, recentlySearch->cityLat, recentlySearch->cityLon);
    }
};

void searchMenu(struct nodeCity *root) {
    int choice;
    char key[30];
    double lat, lon;

    printf("Search based on?\n1. City name\n2. Coordinates\n\nInput>> ");
    do {
        scanf("%d", &choice);
        getchar();
    } while (choice < 1 || choice > 2);

    switch (choice) {
        case 1: {
            recentSearch();
            printf("Enter a city name to search: ");
            scanf("%29[^\n]", key);
            getchar();
            struct nodeCity *curr = search(root, key);
            recentlySearch = curr;
            if (curr == NULL) {
                printf("Data not found!\n");
            } else {
                displayCityDetails(curr);
            }
            break;
        }
        case 2: {
            recentSearch();
            printf("Enter Latitude: ");
            scanf("%lf", &lat);
            getchar();
            printf("Enter Longitude: ");
            scanf("%lf", &lon);
            getchar();
            struct nodeCity *curr = searchCoor(root, lat, lon);
            recentlySearch = curr;
            if (curr == NULL) {
                printf("City not found!\n");
            } else {
                // Display the search result
                displayCityDetails(curr);
            }
            break;
        }
        default:
            printf("Invalid choice!\n");
    }
    utilPressAnyKey();
}

// [4] Delete Function
// TBA

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
        printf("| %-30s | %-30s | %-15.6f | %-15.6f |\n", current->cityName, current->cityDescription, current->cityLat, current->cityLon);
        current = current->next;
    }

    // Print table footer
    for (int i = 0; i < 103; i++) printf("-");
    printf("\n");
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

// [7] Open/Save Data Function
struct nodeCity *readAvlFromFile(FILE *fp, struct nodeCity *root) {
    double lat, lon;
    char cityName[30], cityDescription[100];
    int continentIndex;
    while (fscanf(fp, "%lf,%lf,%29[^,],%99[^,],%d\n", &lat, &lon, cityName, cityDescription, &continentIndex) != EOF) {
        root = insertByCityName(root, &rootByCoor, lat, lon, cityName, cityDescription, continentIndex);
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
