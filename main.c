#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define R 6371

/*Struct Declaration*/
struct nodeCity {
    char cityName[30];
    char cityDescription[100];
    double cityLat;
    double cityLon;
    int continentIndex;
    int height;
    struct nodeCity *left;
    struct nodeCity *right;
};

struct continentNode {
    char cityName[30];
    char cityDescription[100];
    double cityLat;
    double cityLon;
    struct continentNode *next;
};

struct continentNode *continentList[7] = { 0 };

struct nodeCity *rootByCity = NULL; // Untuk AVL Tree City
struct nodeCity *rootByCoor = NULL; // Untuk AVL Tree Koordinat
struct nodeCity *recentlySearch = NULL;

/*Function Declaration*/
int max(int left, int right);
int getHeight(struct nodeCity *root);
int getBFactor(struct nodeCity *root);
struct nodeCity *rightRotate(struct nodeCity *root);
struct nodeCity *leftRotate(struct nodeCity *root);
struct nodeCity *newCityNode(double lat, double lon, const char *newCity, const char *cityDescription, int continentIndex);
struct nodeCity *newCoorNode(double lat, double lon, const char *newCity, const char *cityDescription, int continentIndex);
struct nodeCity *insertByCoor(struct nodeCity *root, double lat, double lon,
                              const char *newCity, const char *cityDescription, int continentIndex);
struct nodeCity *insertByCityName(struct nodeCity *root,
                                  struct nodeCity **rootCoor, double lat,
                                  double lon, const char *key, const char *cityDescription, int continentIndex);

void preorder_viewAllCity(struct nodeCity *root);
void postorder_viewAllCity(struct nodeCity *root);
void inorder_viewAllCity(struct nodeCity *root);
int pushContinentNode(struct continentNode **head, const char *cityName, const char *cityDescription, double lat, double lon);
void printContinent(struct continentNode *continentList[]);

void insertMenu();
void pressanykey();
void recentSearch();

// Math functions
double deg2rad(double deg);
double haversine_distance(double lat1, double lon1, double lat2, double lon2);

// Function untuk mencari berdasarkan City Name
struct nodeCity *search(struct nodeCity *root, char *key);
// Function untuk mencari berdasarkan koordinat
struct nodeCity *searchCoor(struct nodeCity *root, double lat, double lon);

int validateTitle(const char *name);

// Open/Save
void writeAvlToFile(struct nodeCity *root, FILE *fp);
struct nodeCity *readAvlFromFile(FILE *fp, struct nodeCity *root);
void openData();
void saveDataCity();
void saveDataCoor();


/*Driver Function*/
int main(void) {

    openData();
    // openDataCoor();
    int choice;
    do {
        int n;

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
            scanf("%d", &n);
            getchar();
        } while (n < 0 || n > 6);

        switch (n) {

            case 1: {
                insertMenu();
                pressanykey();
                break;
            }

            case 2: {
                if (rootByCity == NULL) {
                    printf("There is no data\n");
                    pressanykey();
                    break;
                }
                printf("1. Sort based on city name\n2. Sort based on "
                       "coordinates\n\nInput>> ");
                scanf("%d", &choice);
                getchar();

                // sort berdasarkan city name
                if (choice == 1) {
                    printf("=======Sort based on city name=======\n");
                    printf("View in which order?\n1. Preorder Traversal\n2. Inorder "
                           "Traversal\n3. Postorder Traversal\nInput>> ");
                    do {
                        scanf("%d", &choice);
                        getchar();
                    } while (choice < 1 || choice > 3);

                    for (int i = 0; i < 94; i++)
                        printf("-");
                    printf("\n");
                    printf("|%-30s|%-30s|%-30s|\n", "City Name", "Latitude", "Longitude");
                    for (int i = 0; i < 94; i++)
                        printf("-");
                    printf("\n");
                    if (choice == 1)
                        preorder_viewAllCity(rootByCity);
                    if (choice == 2)
                        inorder_viewAllCity(rootByCity);
                    if (choice == 3)
                        postorder_viewAllCity(rootByCity);
                }
                    // sort berdasarkan coordinates
                else if (choice == 2) {
                    printf("==========Sort based on coordinates==========\n");
                    printf("View in which order?\n1. Preorder Traversal\n2. Inorder "
                           "Traversal\n3. Postorder Traversal\nInput>> ");
                    do {
                        scanf("%d", &choice);
                        getchar();
                    } while (choice < 1 || choice > 3);

                    for (int i = 0; i < 94; i++)
                        printf("-");
                    printf("\n");
                    printf("|%-30s|%-30s|%-30s|\n", "City Name", "Latitude", "Longitude");
                    for (int i = 0; i < 94; i++)
                        printf("-");
                    printf("\n");
                    if (choice == 1)
                        preorder_viewAllCity(rootByCoor);
                    if (choice == 2)
                        inorder_viewAllCity(rootByCoor);
                    if (choice == 3)
                        postorder_viewAllCity(rootByCoor);
                }

                pressanykey();
                break;
            }

            case 3: {
                int choice;
                char key[30];
                printf("Search based on?\n1. City name\n2. Coordinates\n\nInput>> ");
                do {
                    scanf("%d", &choice);
                    getchar();
                } while (choice < 1 || choice > 2);

                switch (choice) {
                    case 1: {
                        recentSearch();
                        printf("Enter a city name to search: ");
                        scanf("%30[^\n]", key);
                        getchar();
                        struct nodeCity *curr = search(rootByCity, key);
                        recentlySearch = curr;
                        if (curr == NULL) {
                            printf("Data not found!\n");
                        }

                        else {
                            for (int i = 0; i < 94; i++)
                                printf("-");
                            printf("\n");
                            printf("|%-30s|%-30s|%-30s|\n", "City Name", "Latitude", "Longitude");
                            for (int i = 0; i < 94; i++)
                                printf("-");
                            printf("\n");
                            printf("|%-30s|%-30lf|%-30lf|\n", curr->cityName, curr->cityLat,
                                   curr->cityLon);
                            for (int i = 0; i < 94; i++)
                                printf("-");
                            printf("\n");
                        }

                        pressanykey();
                        break;
                    }

                    case 2: {
                        recentSearch();
                        double tlat, tlon;
                        printf("Enter Latitude: ");
                        scanf("%lf", &tlat);
                        getchar();
                        printf("Enter Longitude: ");
                        scanf("%lf", &tlon);
                        getchar();
                        struct nodeCity *curr = searchCoor(rootByCoor, tlat, tlon);
                        recentlySearch = curr;
                        if (curr == NULL) {
                            printf("City not found!\n");
                        }

                        else {
                            for (int i = 0; i < 94; i++)
                                printf("-");
                            printf("\n");
                            printf("|%-30s|%-30s|%-30s|\n", "City Name", "Latitude", "Longitude");
                            for (int i = 0; i < 94; i++)
                                printf("-");
                            printf("\n");
                            printf("|%-30s|%-30lf|%-30lf|\n", curr->cityName, curr->cityLat,
                                   curr->cityLon);
                            for (int i = 0; i < 94; i++)
                                printf("-");
                            printf("\n");
                        }
                    } break;
                }
            }

            case 4:
                // input inorder. delete inorder.
                break;

            case 5: {
                char city1[35];
                char city2[35];
                printf("====== Search the distance from two cities =====\n");
                printf("Note: the distance is in kilometres.\n\n");
                struct nodeCity *curr1;
                struct nodeCity *curr2;

                do {
                    printf("Enter city 1:");
                    scanf("%[^\n]", city1);
                    getchar();
                    curr1 = search(rootByCity, city1);
                    if (curr1 == NULL) {
                        printf("City not found! Please try again!\n\n");
                    }
                } while (curr1 == NULL);

                do {
                    printf("Enter city 2:");
                    scanf("%[^\n]", city2);
                    getchar();
                    curr2 = search(rootByCity, city2);
                    if (curr2 == NULL) {
                        printf("City not found! Please try again!\n\n");
                    }
                } while (curr2 == NULL);

                double result = haversine_distance(curr1->cityLat, curr1->cityLon,
                                                   curr2->cityLat, curr2->cityLon);
                printf("The distance between %s and %s is %lf\n\n", curr1->cityName,
                       curr2->cityName, result);
            }
            case 6:
                printContinent(continentList);
                break;

            case 0:
                saveDataCity();
                saveDataCoor();
                return 0;
                break;

            default: {
                printf("Invalid input\n");
            }
        }
    } while (1);
}

/*Function Definitions*/
int max(int left, int right) {
    if (right > left) {
        return right;
    } else {
        return left;
    }
}

int getHeight(struct nodeCity *root) {
    if (root == NULL) {
        return 0;
    } else {
        return root->height;
    }
}

int getBFactor(struct nodeCity *root) {
    if (root == NULL) {
        return 0;
    } else {
        return getHeight(root->left) - getHeight(root->right);
    }
}

struct nodeCity *rightRotate(struct nodeCity *root) {
    struct nodeCity *newRoot = root->left;
    struct nodeCity *temp = newRoot->right;

    newRoot->right = root;
    root->left = temp;

    root->height = max(getHeight(root->left), getHeight(root->right)) + 1;
    newRoot->height =
            max(getHeight(newRoot->left), getHeight(newRoot->right)) + 1;

    return newRoot;
}

struct nodeCity *leftRotate(struct nodeCity *root) {
    struct nodeCity *newRoot = root->right;
    struct nodeCity *temp = newRoot->left;

    newRoot->left = root;
    root->right = temp;

    root->height = max(getHeight(root->left), getHeight(root->right)) + 1;
    newRoot->height =
            max(getHeight(newRoot->left), getHeight(newRoot->right)) + 1;

    return newRoot;
}

struct nodeCity *newCityNode(double lat, double lon, const char *newCity, const char *cityDescription, int continentIndex) {
    struct nodeCity *current = (struct nodeCity *)malloc(sizeof(struct nodeCity));
    strcpy(current->cityName, newCity);
    strcpy(current->cityDescription, cityDescription);
    current->left = NULL;
    current->right = NULL;
    current->height = 1;
    current->cityLat = lat;
    current->cityLon = lon;
    current->continentIndex = continentIndex;
    return current;
}

struct nodeCity *newCoorNode(double lat, double lon, const char *newCity, const char *cityDescription, int continentIndex) {
    struct nodeCity *current = (struct nodeCity *)malloc(sizeof(struct nodeCity));
    strcpy(current->cityName, newCity);
    strcpy(current->cityDescription, cityDescription);
    current->left = NULL;
    current->right = NULL;
    current->height = 1;
    current->cityLat = lat;
    current->cityLon = lon;
    current->continentIndex = continentIndex;

    return current;
}

// Insert node ke AVL Tree Coor
struct nodeCity *insertByCoor(struct nodeCity *root, double lat, double lon,
                              const char *newCity, const char *cityDescription, int continentIndex) {
    if (root == NULL) {
        return newCoorNode(lat, lon, newCity, cityDescription, continentIndex);
    }

    if (lat < root->cityLat || (lat == root->cityLat && lon < root->cityLon)) {
        // Kalau latitude lebih kecil dari latitude current node, atau jika latitude
        // sama tapi longitude lebih kecil dari current node longitude, buat node ke
        // subtree kiri
        root->left = insertByCoor(root->left, lat, lon, newCity, cityDescription, continentIndex);
    } else if (lat > root->cityLat ||
               (lat == root->cityLat && lon > root->cityLon)) {
        // Kalau latitude lebih besar dari latitude current node, atau jika latitude
        // sama tapi longitude lebih besar dari current node longitude, buat node ke
        // subtree kanan
        root->right = insertByCoor(root->right, lat, lon, newCity, cityDescription, continentIndex);
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
}

// Insert node ke AVL Tree City
struct nodeCity *insertByCityName(struct nodeCity *root,
                                  struct nodeCity **rootCoor, double lat,
                                  double lon, const char *key, const char *cityDescription, int continentIndex) {
    /* Insert a new city into the tree */
    if (root == NULL) {
        root = newCityNode(lat, lon, key, cityDescription, continentIndex);
        pushContinentNode(&continentList[continentIndex - 1], key, cityDescription, lat, lon);
        // Buat node untuk AVL tree City sekaligus buat node untuk AVL tree Coor
        *rootCoor = insertByCoor(*rootCoor, lat, lon, key, cityDescription, continentIndex);
    } else if (strcmp(key, root->cityName) < 0) {
        root->left = insertByCityName(root->left, rootCoor, lat, lon,
                                      key, cityDescription, continentIndex); // Insert to the left subtree
    } else {
        root->right = insertByCityName(root->right, rootCoor, lat, lon,
                                       key, cityDescription, continentIndex); // Insert to the right subtree
    }

    root->height = max(getHeight(root->left), getHeight(root->right)) + 1;
    int bFactor = getBFactor(root);

    // LL Case
    if (bFactor > 1 && strcmp(key, root->left->cityName) < 0) {
        return rightRotate(root);
    }
    // RR Case
    if (bFactor < -1 && strcmp(key, root->right->cityName) > 0) {
        return leftRotate(root);
    }
    // LR Case
    if (bFactor > 1 && strcmp(key, root->left->cityName) > 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    // RL Case
    if (bFactor < -1 && strcmp(key, root->right->cityName) < 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    return root;
}

void inorder_viewAllCity(
        struct nodeCity *root) { // untuk membaca AVL dan menampilkan
    // data yang ada di dalamnya
    if (root == NULL)
        return;
    inorder_viewAllCity(root->left);

    printf("|%-30s|%-30lf|%-30lf|\n", root->cityName, root->cityLat,
           root->cityLon);
    for (int i = 0; i < 94; i++)
        printf("-");
    printf("\n");

    inorder_viewAllCity(root->right);
}

void preorder_viewAllCity(
        struct nodeCity *root) { // untuk membaca AVL dan menampilkan
    // data yang ada di dalamnya
    if (root == NULL)
        return;
    printf("|%-30s|%-30lf|%-30lf|\n", root->cityName, root->cityLat,
           root->cityLon);
    for (int i = 0; i < 94; i++)
        printf("-");
    printf("\n");

    preorder_viewAllCity(root->left);

    preorder_viewAllCity(root->right);
}

void postorder_viewAllCity(
        struct nodeCity *root) { // untuk membaca AVL dan menampilkan
    // data yang ada di dalamnya
    if (root == NULL)
        return;

    postorder_viewAllCity(root->left);

    postorder_viewAllCity(root->right);

    printf("|%-30s|%-30lf|%-30lf|\n", root->cityName, root->cityLat,
           root->cityLon);
    for (int i = 0; i < 94; i++)
        printf("-");
    printf("\n");
}

int validateTitle(const char *name) {
    if (strlen(name) < 5 || strlen(name) > 30)
        return 1;
    return 0;
}

void insertMenu() {
    char key[30], cityDescription[100];
    double lat, lon;
    int continentIndex;
    struct nodeCity *cityNode;
    struct nodeCity *coorNode;

    do {
        printf("Input City Name [5-30]: ");
        scanf("%30[^\n]%*c", key);
        cityNode = search(rootByCity, key);
        if (validateTitle(key)) {
            printf("City Name must be between 5 and 30 characters!\n");
        } else if (cityNode != NULL) {
            printf("City Name already exists!\n");
        }
    } while (validateTitle(key) || cityNode != NULL);

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

    rootByCity = insertByCityName(rootByCity, &rootByCoor, lat, lon, key, cityDescription, continentIndex);
    printf("City successfully added!\n");
}

// search dengan city sebagai key
struct nodeCity *search(struct nodeCity *root, char *key) {
    if (root == NULL || strcasecmp(root->cityName, key) == 0) {
        return root;
    }

    if (strcasecmp(root->cityName, key) < 0) {
        return search(root->right, key);
    }

    return search(root->left, key);
}

struct nodeCity *searchCoor(struct nodeCity *root, double lat, double lon) {
    /* Cek apabila root = NULL atau koordinat match dengan root, kembalikan root
    karena nilai koordinat match */
    if (root == NULL || (lat == root->cityLat && lon == root->cityLon)) {
        return root;
    }

    /* Kalau target lattitude lebih besar dari lattitude sekarang atau jika
    lattitude target sama dengan lattitude sekarang tapi longitude target lebih
    besar dari longitude sekarang, cari di subtree sebelah kanan */
    if (lat > root->cityLat || (lat == root->cityLat && lon > root->cityLon)) {
        return searchCoor(root->right, lat, lon);
    } else {
        /* Kalau target lattitude lebih kecil dari lattitude sekarang atau jika
        lattitude target sama dengan lattitude sekarang tapi longitude target
        lebih kecil dari longitude sekarang, cari di subtree sebelah kiri */
        return searchCoor(root->left, lat, lon);
    }
}

void pressanykey() {
    printf("Press any key to continue...");
    getchar();
    system("cls");
}

// preorder
void writeAvlToFile(struct nodeCity *root, FILE *fp) {
    if (root == NULL) {
        return;
    }
    fprintf(fp, "%lf,%lf,%s,%s,%d\n", root->cityLat, root->cityLon, root->cityName, root->cityDescription, root->continentIndex);
    writeAvlToFile(root->left, fp);
    writeAvlToFile(root->right, fp);
}

struct nodeCity *readAvlFromFile(FILE *fp, struct nodeCity *root) {
    double lat, lon;
    char cityName[30], cityDescription[100];
    int continentIndex;
    while (fscanf(fp, "%lf,%lf,%29[^,],%99[^,],%d\n", &lat, &lon, cityName, cityDescription, &continentIndex) != EOF) {
        root = insertByCityName(root, &rootByCoor, lat, lon, cityName, cityDescription, continentIndex);
    }
    return root;
}

void openData() {
    FILE *fp = fopen("data_city.txt", "r");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }
    rootByCity = readAvlFromFile(fp, rootByCity);
    fclose(fp);
}

void saveDataCoor() {
    FILE *fp = fopen("data_coor.txt", "w");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }
    writeAvlToFile(rootByCoor, fp);
    fclose(fp);
    printf("Data Saved!");
}

void saveDataCity() {
    FILE *fp = fopen("data_city.txt", "w");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }
    writeAvlToFile(rootByCity, fp);
    fclose(fp);
    printf("Data Saved!");
}

void recentSearch() {
    if(recentlySearch == NULL) {
        return;
    } else {
        printf("Recent Search : %s (%f, %f)\n", recentlySearch->cityName, recentlySearch->cityLat, recentlySearch->cityLon);
    }
}

int pushContinentNode(struct continentNode **head, const char *cityName, const char *cityDescription, double lat, double lon) {
    struct continentNode *newNode = malloc(sizeof(struct continentNode));
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
}

void printContinent(struct continentNode *continentList[]) {
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

    struct continentNode *current = continentList[continentIndex - 1];
    while (current != NULL) {
        printf("City Name: %s\n", current->cityName);
        printf("City Description: %s\n", current->cityDescription);
        printf("Latitude: %lf\n", current->cityLat);
        printf("Longitude: %lf\n", current->cityLon);
        printf("----------------------\n");
        current = current->next;
    }
}

// math functions

double deg2rad(double deg) { return deg * (M_PI / 180); }

double haversine_distance(double lat1, double lon1, double lat2, double lon2) {
    lat1 = deg2rad(lat1);
    lon1 = deg2rad(lon1);
    lat2 = deg2rad(lat2);
    lon2 = deg2rad(lon2);
    return acos(sin(lat1) * sin(lat2) +
                cos(lat1) * cos(lat2) * cos(lon2 - lon1)) *
           6371;
}