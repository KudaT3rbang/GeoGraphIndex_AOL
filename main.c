#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>
#define M_PI 3.14159265358979323846
#include <windows.h>
#undef max
#include <conio.h> // For Windows

// COLORS
#define red   "\x1B[31m"
#define grn   "\x1B[32m"
#define yel   "\x1B[33m"
#define blu   "\x1B[34m"
#define mag   "\x1B[35m"
#define cyn   "\x1B[36m"
#define wht   "\x1B[37m"
#define reset "\x1B[0m"

#define EARTH_RADIUS 6371.0 // Earth's radius in kilometers

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
void printTableHeader();
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

// [10] Radius Search Function
double haversine(double lat1, double lon1, double lat2, double lon2);
void findCitiesWithinRadius(struct nodeCity *root, double lat, double lon, double radius);
void searchCitiesByRadius(struct nodeCity *root);

/* Main Function */
void main()
{
    openData();
    int choice;
    char menu = '\0';
    int index = 1;
    system("cls");
    do{

        do
        {
            system("cls");
            switch (index)
            {
                case 1:
                {
                    printf("\nWelcome to GeoGraphIndex Program\n");
                    printf(blu);
                    printf("-----------------------------------\n"); printf(reset);
                    printf(grn"  > Add a city\n"reset);
                    printf("View all cities\n");
                    printf("Find a city\n");
                    printf("Delete a city\n");
                    printf("Find distance of two cities\n");
                    printf("View cities based on continent\n");
                    printf("Find cities within radius\n");
                    printf("Edit a city\n");
                    printf("Exit\n");
                    printf(blu);
                    printf("-----------------------------------\n"); printf(reset);
                    printf(yel"\n\nNote : Use Arrow Key to navigate or WASD\n"); printf(reset);
                    break;
                }
                case 2:
                {
                    printf("\nWelcome to GeoGraphIndex Program\n");
                    printf(blu);
                    printf("-----------------------------------\n"); printf(reset);
                    printf("Add a city\n");
                    printf(grn"  > View all cities\n"reset);
                    printf("Find a city\n");
                    printf("Delete a city\n");
                    printf("Find distance of two cities\n");
                    printf("View cities based on continent\n");
                    printf("Find cities within radius\n");
                    printf("Edit a city\n");
                    printf("Exit\n");
                    printf(blu);
                    printf("-----------------------------------\n"); printf(reset);
                    printf(yel"\n\nNote : Use Arrow Key to navigate or WASD\n"); printf(reset);
                    break;
                }
                case 3:
                {
                    printf("\nWelcome to GeoGraphIndex Program\n");
                    printf(blu);
                    printf("-----------------------------------\n"); printf(reset);
                    printf("Add a city\n");
                    printf("View all cities\n");
                    printf(grn"  > Find a city\n"reset);
                    printf("Delete a city\n");
                    printf("Find distance of two cities\n");
                    printf("View cities based on continent\n");
                    printf("Find cities within radius\n");
                    printf("Edit a city\n");
                    printf("Exit\n");
                    printf(blu);
                    printf("-----------------------------------\n"); printf(reset);
                    printf(yel"\n\nNote : Use Arrow Key to navigate or WASD\n"); printf(reset);
                    break;
                }
                case 4:
                {
                    printf("\nWelcome to GeoGraphIndex Program\n");
                    printf(blu);
                    printf("-----------------------------------\n"); printf(reset);
                    printf("Add a city\n");
                    printf("View all cities\n");
                    printf("Find a city\n");
                    printf(grn"  > Delete a city\n"reset);
                    printf("Find distance of two cities\n");
                    printf("View cities based on continent\n");
                    printf("Find cities within radius\n");
                    printf("Edit a city\n");
                    printf("Exit\n");
                    printf(blu);
                    printf("-----------------------------------\n"); printf(reset);
                    printf(yel"\n\nNote : Use Arrow Key to navigate or WASD\n"); printf(reset);
                    break;
                }
                case 5:
                {
                    printf("\nWelcome to GeoGraphIndex Program\n");
                    printf(blu);
                    printf("-----------------------------------\n"); printf(reset);
                    printf("Add a city\n");
                    printf("View all cities\n");
                    printf("Find a city\n");
                    printf("Delete a city\n");
                    printf(grn"  > Find distance of two cities\n"reset);
                    printf("View cities based on continent\n");
                    printf("Find cities within radius\n");
                    printf("Edit a city\n");
                    printf("Exit\n");
                    printf(blu);
                    printf("-----------------------------------\n"); printf(reset);
                    printf(yel"\n\nNote : Use Arrow Key to navigate or WASD\n"); printf(reset);
                    break;
                }
                case 6:
                {
                    printf("\nWelcome to GeoGraphIndex Program\n");
                    printf(blu);
                    printf("-----------------------------------\n"); printf(reset);
                    printf("Add a city\n");
                    printf("View all cities\n");
                    printf("Find a city\n");
                    printf("Delete a city\n");
                    printf("Find distance of two cities\n"reset);
                    printf(grn"  > View cities based on continent\n"reset);
                    printf("Find cities within radius\n");
                    printf("Edit a city\n");
                    printf("Exit\n");
                    printf(blu);
                    printf("-----------------------------------\n"); printf(reset);
                    printf(yel"\n\nNote : Use Arrow Key to navigate or WASD\n"); printf(reset);
                    break;
                }
                case 7:
                {
                    printf("\nWelcome to GeoGraphIndex Program\n");
                    printf(blu);
                    printf("-----------------------------------\n"); printf(reset);
                    printf("Add a city\n");
                    printf("View all cities\n");
                    printf("Find a city\n");
                    printf("Delete a city\n");
                    printf("Find distance of two cities\n"reset);
                    printf("View cities based on continent\n"reset);
                    printf(grn"  > Find cities within radius\n"reset);
                    printf("Edit a city\n");
                    printf("Exit\n");
                    printf(blu);
                    printf("-----------------------------------\n"); printf(reset);
                    printf(yel"\n\nNote : Use Arrow Key to navigate or WASD\n"); printf(reset);
                    break;
                }
                case 8:
                {
                    printf("\nWelcome to GeoGraphIndex Program\n");
                    printf(blu);
                    printf("-----------------------------------\n"); printf(reset);
                    printf("Add a city\n");
                    printf("View all cities\n");
                    printf("Find a city\n");
                    printf("Delete a city\n");
                    printf("Find distance of two cities\n"reset);
                    printf("View cities based on continent\n"reset);
                    printf("Find cities within radius\n"reset);
                    printf(grn"  > Edit a city\n"reset);
                    printf("Exit\n");
                    printf(blu);
                    printf("-----------------------------------\n"); printf(reset);
                    printf(yel"\n\nNote : Use Arrow Key to navigate or WASD\n"); printf(reset);
                    break;
                }
                case 9:
                {
                    printf("\nWelcome to GeoGraphIndex Program\n");
                    printf(blu);
                    printf("-----------------------------------\n"); printf(reset);
                    printf("Add a city\n");
                    printf("View all cities\n");
                    printf("Find a city\n");
                    printf("Delete a city\n");
                    printf("Find distance of two cities\n"reset);
                    printf("View cities based on continent\n"reset);
                    printf("Find cities within radius\n"reset);
                    printf("Edit a city\n"reset);
                    printf(red"Exit\n"reset);
                    printf(blu);
                    printf("-----------------------------------\n"); printf(reset);
                    printf(yel"\n\nNote : Use Arrow Key to navigate or WASD\n"); printf(reset);
                    break;
                }
            }
            fflush(stdin);
            menu = _getch();
            if(menu == 72 || menu == 'w' || menu == 'W') //UP
            {
                index--;
                if(index < 1)
                {
                    index = 9;
                }
            }
            else if(menu == 80 || menu == 's' || menu == 'S') //DOWN
            {
                index++;
                if(index > 9)
                {
                    index = 1;
                }
            }
            else if(menu == 224)
            {
                menu = _getch();
            }
        } while (menu != '\r' );


        switch(index) {
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
                utilPressAnyKey();
                break;
            case 6:
                printContinent(continentList);
                break;
            case 7:
                searchCitiesByRadius(rootByCity);
                break;
            case 8:
                updateCityDescription();
                break;
            case 9:
                saveDataCity();
                saveDataCoor();
                utilPressAnyKey();
                break;
            default:
                printf("Invalid input!\n");
                break;
        }


        system("cls");
    } while (index != 9);
    return;
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
    printf("%s=====%s Add a city %s=====%s\n\n", blu, reset, blu, reset);
    char cityName[30], cityDescription[1000];
    double lat, lon;
    int continentIndex;
    struct nodeCity *cityNode;
    struct nodeCity *coorNode;

    do {
        printf(yel"Input City Name [1-30]: "reset);
        if (fgets(cityName, sizeof(cityName), stdin) == NULL) {
            fprintf(stderr, "Error reading city name.\n");
            return;
        }
        size_t len = strlen(cityName);
        if (len > 0 && cityName[len - 1] == '\n') {
            cityName[len - 1] = '\0';
        }
        cityNode = search(rootByCity, cityName);
        if (utilValidateTitle(cityName)) {
            printf(red"City Name must be between 1 and 30 characters!\n\n"reset);
        } else if (cityNode != NULL) {
            printf(red"City Name already exists!\n\n"reset);
        }
    } while (utilValidateTitle(cityName) || cityNode != NULL);

    printf(yel"Input City Description: "reset);
    scanf("%1000[^\n]%*c", cityDescription);

    do {
        printf(yel"Input City Latitude: "reset);
        scanf("%lf", &lat);
        getchar();

        printf(yel"Input City Longitude: "reset);
        scanf("%lf", &lon);
        getchar();

        coorNode = searchCoor(rootByCoor, lat, lon);
        if (coorNode != NULL) {
            printf(red"Coordinate already exists!\n\n"reset);
        }
    } while (coorNode != NULL);


    printf(yel"\n\nInput Continent :\n"reset);
    printf("1. North America\n");
    printf("2. South America\n");
    printf("3. Europe\n");
    printf("4. Africa\n");
    printf("5. Asia\n");
    printf("6. Australia\n");
    printf("7. Antartica\n");
    printf(yel"Input >> "reset);
    do {
        scanf("%d", &continentIndex);
        getchar();
    } while (continentIndex < 0 || continentIndex > 5);

    rootByCity = insertByCityName(rootByCity, &rootByCoor, lat, lon, cityName, cityDescription, continentIndex);
    NumberOfCities++;
    printf(yel"City added successfully!\n"reset);
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

void printTableHeader() {
    for (int i = 0; i < 136; i++) printf("-");
    printf("\n");
    printf("| %s%-30s%s | %s%-30s%s | %s%-20s%s | %s%-20s%s | %s%-20s%s |\n", mag,"City Name",reset, mag,"City Description",reset,mag, "Latitude",reset,mag, "Longitude",reset, mag,"Continent",reset);
    for (int i = 0; i < 136; i++) printf("-");
    printf("\n");
}


void sortMenu() {
    char menu = '\0', select= '\0';
    int index = 1, currentPage = 0, maxPage;
    struct nodeCity* cities[NumberOfCities];
    int indexinorder = 0;

    system("cls");

    if(rootByCity == NULL) {
        printf("There is no data!\n");
        utilPressAnyKey();
        return;
    }

    // Print menu and handle navigation
    do {
        system("cls");
        printf("%s========%s View and sort all cities %s========%s\n\n", blu, reset, blu, reset);
        if (index == 1) {
            printf(grn"    > Sort based on city name\n"reset);
            printf("Sort based on coordinates\n\n");
        } else {
            printf("Sort based on city name\n");
            printf(grn"    > Sort based on coordinates\n\n"reset);
        }

        menu = _getch();
        if(menu == 72 || menu == 'w' || menu == 'W') // UP
        {
            index--;
            if(index < 1)
            {
                index = 1;
            }
        }
        else if(menu == 80 || menu == 's' || menu == 'S')
        { // DOWN
            index++;
            if(index > 2)
            {
                index = 2;
            }
        }
    } while (menu != '\r');

    // Perform inOrderTraversal based on the selected index
    if (index == 1) {
        inOrderTraversal(rootByCity, cities, &indexinorder);
    } else {
        inOrderTraversal(rootByCoor, cities, &indexinorder);
    }

    menu = '\0', select= '\0';
     do {
        system("cls");
        printf("%s========%s View and sort all cities %s========%s\n\n", blu, reset, blu, reset);
        if (index == 1) {
            printf(grn"    > Ascending\n"reset);
            printf("Descending\n\n");
        } else {
            printf("Ascending\n");
            printf(grn"    > Descending\n\n"reset);
        }

        menu = _getch();
        if(menu == 72 || menu == 'w' || menu == 'W') // UP
        {
            index--;
            if(index < 1)
            {
                index = 1;
            }
        }
        else if(menu == 80 || menu == 's' || menu == 'S')
        { // DOWN
            index++;
            if(index > 2)
            {
                index = 2;
            }
        }
    } while (menu != '\r');


        // Calculate maxPage for pagination
        maxPage = (NumberOfCities + 19) / 20 - 1;

        // Page navigation
        fflush(stdin);
        do {
            if (index == 1) {
                displayPage(cities, currentPage * 20, NumberOfCities);
            } else {
                displayPageDescending(cities, currentPage * 20, NumberOfCities);
            }

            select = _getch(); // Use _getch() for consistency
            if(select == 72 || select == 'W' || select == 'w') {
                currentPage = (currentPage > 0) ? currentPage - 1 : 0;
            } else if(select == 80 || select == 'S' || select == 's') {
                currentPage = (currentPage < maxPage) ? currentPage + 1 : maxPage;
            }
        } while(select != '\r');
}





void displayPage(struct nodeCity** cities, int startIndex, int totalCities) {
    system("cls"); // Clear the screen
    printTableHeader();
    int endIndex = startIndex + 20 < totalCities ? startIndex + 20 : totalCities;
    for (int i = startIndex; i < endIndex; i++) {
        if(strlen(cities[i]->cityDescription)<=30)
            printf("| %-30s | %-30s | %-20.6f | %-20.6f | %-20s |\n", cities[i]->cityName, cities[i]->cityDescription, cities[i]->cityLat, cities[i]->cityLon, getContinentName(cities[i]->continentIndex));
        else
            printf("| %-30s | %-26.26s.... | %-20.6f | %-20.6f | %-20s |\n", cities[i]->cityName, cities[i]->cityDescription, cities[i]->cityLat, cities[i]->cityLon, getContinentName(cities[i]->continentIndex));

		for (int i = 0; i < 136; i++) printf("-");
        printf("\n");
    }

    printf("\n\n");
    printf(yel"Displaying %d - %d out of %d\n"reset, startIndex + 1, endIndex, totalCities);
    printf(blu"\n\n [W / ^] Previous  [S / v] Next  [enter] Back to main menu\n"reset);

}


void displayPageDescending(struct nodeCity** cities, int startIndex, int totalCities) {
    system("cls"); // Clear the screen
    printTableHeader();
    int endIndex = startIndex + 20 < totalCities ? startIndex + 20 : totalCities;
    // Calculate the starting and ending indices for the loop in descending order
    int startDescIndex = totalCities - 1 - startIndex;
    int endDescIndex = startDescIndex - (endIndex - startIndex);

    for (int i = startDescIndex; i > endDescIndex; i--) {
        if(strlen(cities[i]->cityDescription)<=30)
            printf("| %-30s | %-30s | %-20.6f | %-20.6f | %-20s |\n", cities[i]->cityName, cities[i]->cityDescription, cities[i]->cityLat, cities[i]->cityLon, getContinentName(cities[i]->continentIndex));
        else
            printf("| %-30s | %-26.26s.... | %-20.6f | %-20.6f | %-20s |\n", cities[i]->cityName, cities[i]->cityDescription, cities[i]->cityLat, cities[i]->cityLon, getContinentName(cities[i]->continentIndex));

        for (int j = 0; j < 136; j++) printf("-");
        printf("\n");
    }
    // Adjust the displayed range of cities to match the descending order
    int displayedStart = totalCities - startDescIndex;
    int displayedEnd = totalCities - endDescIndex - 1;
    printf(yel"Displaying %d - %d out of %d\n"reset, displayedStart, displayedEnd, totalCities);
    printf(blu"\n\n [^] Previous  [v] Next  [enter] Back to main menu\n"reset);
}

void inOrderTraversal(struct nodeCity* root, struct nodeCity** cities, int* index) {
    if (root != NULL) {
        inOrderTraversal(root->left, cities, index);
        cities[*index] = root;
        (*index)++;
        inOrderTraversal(root->right, cities, index);
    }
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
                printf("| %-30s | %-30s | %-20.6f | %-20.6f | %-20s |\n", root->cityName, root->cityDescription, root->cityLat, root->cityLon, getContinentName(root->continentIndex));
            else
                printf("| %-30s | %-26.26s.... | %-20.6f | %-20.6f | %-20s |\n", root->cityName, root->cityDescription, root->cityLat, root->cityLon, getContinentName(root->continentIndex));

            for (int i=0; i<136; i++) printf("-");
            printf("\n");
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
        printf(yel"City Found!\n"reset);
        printf(mag"===========================\n"reset);
        printf("City Name : %s\n", city->cityName);
        printf("City Description :\n");
        printf("%s\n", city->cityDescription);
        printf("Latitude : %lf\n", city->cityLat);
        printf("Longitude : %lf\n", city->cityLon);
        printf("Continent : %s\n", getContinentName(city->continentIndex));
        printf(mag"===========================\n"reset);
    }
}

struct nodeCity *searchCityMenu() {
    char pref[100] = "";
    char temp = '\0';
    prefixCount = 0;
    do {
        system("cls");
        printf("%s=======%s Find a city %s=======\n\n%s", blu, reset, blu, reset);
        printf(yel);
        recentSearch();
        printf("\nNote: press enter after typing the full city name\n\n");
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
	system("cls");
	printf("%s=====%s Find a city %s=====%s\n\n", blu, reset, blu, reset);
    double lat, lon;
    recentSearch();
    printf("\n");
    printf(yel"Enter Latitude: "reset);
    scanf("%lf", &lat);
    getchar();
    printf(yel"Enter Longitude: "reset);
    scanf("%lf", &lon);
    getchar();

    struct nodeCity *curr = searchCoor(rootByCoor, lat, lon);
    if (curr == NULL) {
        printf("City not found!\n");
    } else {
    	system("cls");
        displayCityDetails(curr);
    }
}

void searchMenu(struct nodeCity *root) {

    system("cls");
    int choice;
    char key[30];
    double lat, lon;
    printf("%s=====%s Find a city %s=====%s\n\n", blu, reset, blu, reset);
    printf(yel"Search based on?\n"reset);
	printf("1. City name\n2. Coordinates\n\n");
	printf(mag"Input>> "reset);
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
    struct nodeCity *temp = NULL;

    printf("%s=====%s Delete a city %s=====%s\n\n", blu, reset, blu, reset);
    printf("%sDelete based on?%s\n1. City name\n2. Coordinates\n\n%sInput>>%s ", yel, reset, mag, reset);
    do {
        scanf("%d", &choice);
        getchar();
    } while (choice < 1 || choice > 2);

    switch (choice) {
        case 1:
            printf(yel"\n\n\nEnter a city name to delete: "reset);
            scanf("%29[^\n]", cityName);
            getchar();
            struct nodeCity *nodeByName = search(rootByCity, cityName);
            if (nodeByName) {
                rootByCity = deleteNodeByCity(rootByCity, cityName);
                rootByCoor = deleteNodeByCoor(rootByCoor, nodeByName->cityLat, nodeByName->cityLon);
                NumberOfCities--;
                printf(yel"\n\nCity successfully deleted!\n"reset);
            } else {
                printf(red"\n\nCity not found!\n"reset);
            }
            break;
        case 2:
            printf(yel"\n\n\nEnter Latitude: "reset);
            scanf("%lf", &lat);
            getchar();
            printf(yel"Enter Longitude: "reset);
            scanf("%lf", &lon);
            getchar();
            struct nodeCity *nodeByCoor = searchCoor(rootByCoor, lat, lon);
            if (nodeByCoor) {
                rootByCoor = deleteNodeByCoor(rootByCoor, lat, lon);
                rootByCity = deleteNodeByCity(rootByCity, nodeByCoor->cityName);
                NumberOfCities--;
                printf(yel"\n\nCity successfully deleted!\n"reset);
            } else {
                printf(red"\n\nCity not found!\n"reset);
            }
            break;
        default:
            printf(red"Invalid choice!\n\n"reset);
    }
    NumberOfCities--;
    utilPressAnyKey();
}

// [5] Function For Array Of Linked List (Continent)
int pushNodeContinent(struct nodeContinent **head, const char *cityName, const char *cityDescription, double lat, double lon) {
    struct nodeContinent *newNode = malloc(sizeof(struct nodeContinent));
    if (newNode == NULL) {
        printf(red"Memory allocation failed\n"reset);
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
	system("cls");
    printf("%s=====%s View cities in a continent %s=====%s\n\n", blu, reset, blu, reset);
    int continentIndex;
    const char *continentNames[] = {"North America", "South America", "Europe", "Africa", "Asia", "Australia", "Antarctica"};

    printf(yel"Select a continent to view:\n"reset);
    for (int i = 0; i < 7; ++i) {
        printf("%d. %s\n", i + 1, continentNames[i]);
    }
    printf(yel"Enter the index of the continent: "reset);
    scanf("%d", &continentIndex);
    getchar();

    if (continentIndex < 1 || continentIndex > 7) {
        printf(red"Invalid continent index!\n"reset);
        utilPressAnyKey();
        return;
    }
    system("cls");
    printf("%s=====%s View cities in a continent %s=====%s\n\n", blu, reset, blu, reset);
    printf(mag"\n\nCities in %s:\n\n"reset, continentNames[continentIndex - 1]);

    // Print table header
    for (int i = 0; i < 103; i++) printf("-");
    printf("\n");
    printf("| %-30s | %-30s | %-15s | %-15s |\n", "City Name", "City Description","Latitude","Longitude");
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
    printf(yel"\n\nPress Enter to continue..."reset);
    getchar();
    system("cls");
};

int utilValidateTitle(const char *name) {
    return (strlen(name) < 1 || strlen(name) > 30) ? 1 : 0;
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
        printf(red"No file found!\n"reset);
        return;
    }
    rootByCity = readAvlFromFile(fp, rootByCity);
    fclose(fp);
};

void saveDataCity() {
    FILE *fp = fopen("data_coor.txt", "w");
    if (fp == NULL) {
        printf(red"Failed to save data!\n"reset);
        return;
    }
    writeAvlToFile(rootByCoor, fp);
    fclose(fp);
    printf(yel"\n\nCity AVL Tree Saved\n"reset);
};

void saveDataCoor() {
    FILE *fp = fopen("data_city.txt", "w");
    if (fp == NULL) {
        printf(red"Failed to save data!\n"reset);
        return;
    }
    writeAvlToFile(rootByCity, fp);
    fclose(fp);
    printf(yel"Coordinates AVL Tree Saved\n"reset);
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
    system("cls");
    char city1[35], city2[35];
    printf("%s======%s Search the distance between two cities %s=====%s\n", blu, reset, blu, reset);
    printf(yel"\n\nNote: the distance is in kilometres.\n\n"reset);

    struct nodeCity *curr1, *curr2;

    do {
        printf(yel"Enter city 1: "reset);
        scanf("%34[^\n]", city1);
        getchar();
        curr1 = search(rootByCity, city1);
        if (curr1 == NULL) {
            printf(red"City not found! Please try again.\n\n"reset);
        }
    } while (curr1 == NULL);

    do {
        printf(yel"Enter city 2: "reset);
        scanf("%34[^\n]", city2);
        getchar();
        curr2 = search(rootByCity, city2);
        if (curr2 == NULL) {
            printf(red"City not found! Please try again.\n\n"reset);
        }
    } while (curr2 == NULL);

    // Calculate and display the distance
    double result = distanceCheck(curr1->cityLat, curr1->cityLon, curr2->cityLat, curr2->cityLon);
    printf("\nThe distance between %s and %s is %.2lf kilometers.\n\n", curr1->cityName, curr2->cityName, result);
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

        printf("%s \nEnter new description for %s: %s", yel,cityByName->cityName,reset);
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

        printf("\nCity description updated successfully!\n");
        utilPressAnyKey();
    }
}

// [10] Radius Search Function
double haversine(double lat1, double lon1, double lat2, double lon2) {
    double dlat = deg2rad(lat2 - lat1);
    double dlon = deg2rad(lon2 - lon1);

    double a = sin(dlat / 2) * sin(dlat / 2) +
               cos(deg2rad(lat1)) * cos(deg2rad(lat2)) *
               sin(dlon / 2) * sin(dlon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

    return EARTH_RADIUS * c;
}

void findCitiesWithinRadius(struct nodeCity *root, double lat, double lon, double radius) {
    if (root == NULL) {
        return;
    }

    // Calculate the distance from the given point to the current city
    double distance = haversine(lat, lon, root->cityLat, root->cityLon);

    // If the distance is within the specified radius, print the city
    if (distance <= radius) {

        printf("%sCity:%s %s, %sDistance:%s %.2f km\n", mag,reset, root->cityName, mag,reset, distance);
    }

    // Recursively search in the left and right subtrees
    findCitiesWithinRadius(root->left, lat, lon, radius);
    findCitiesWithinRadius(root->right, lat, lon, radius);
}

void searchCitiesByRadius(struct nodeCity *root) {
	system("cls");
	printf("%s=====%s Find cities within radius %s=====%s\n\n", blu, reset, blu, reset);

    double lat, lon, radius;

    // Get user input for latitude, longitude, and radius
    printf(yel"Enter latitude: "reset);
    scanf("%lf", &lat);
    printf(yel"Enter longitude: "reset);
    scanf("%lf", &lon);
    printf(yel"Enter radius (in km): "reset);
    scanf("%lf", &radius);

    // Find and print cities within the specified radius
    findCitiesWithinRadius(root, lat, lon, radius);
    getchar();
    utilPressAnyKey();
}