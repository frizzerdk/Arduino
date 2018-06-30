#include <EEPROM.h>
//=========== Storage constants ===================
#define EEPROM_SIZE 1024
#define TOTAL_USERS_ADDR 10
#define FIRST_OPEN_ADDR 9
#define HAS_BEEN_OPEN 10
#define USER_DATA_ADDR 500
#define ALIAS_LENGTH 10
#define MEASUREMENTS 10
#define MEASUREMENT_BYTES 4
int USER_BYTES = ALIAS_LENGTH + MEASUREMENTS * MEASUREMENT_BYTES;
const int MAX_USERS  = (EEPROM_SIZE - USER_DATA_ADDR) / USER_BYTES;
//=========== Structs =============================
typedef struct user {
  char alias[ALIAS_LENGTH];
  int measurements[MEASUREMENTS] = {0};
} user;
//=========== Pins ================================
#define pinButton 8
#define pinLed 13
//=========== Menus ===============================
#define MENU_MAIN 0
#define MENU_USERS 1
#define MENU_USERS_DELETE 2
int currentMenu = MENU_MAIN;
//=========== Variables ===========================
byte currentUser = 0;
byte totalUsers = 0;
user users[MEASUREMENTS] ;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(pinButton, INPUT);
  pinMode(pinLed, OUTPUT);
  // load data

  totalUsers = EEPROM.read(TOTAL_USERS_ADDR);

  if (EEPROM.read(FIRST_OPEN_ADDR) != HAS_BEEN_OPEN) {
    totalUsers = 0;
    newUser();

  } else {
    //load user data
    loadUsers();
  }
  //  Serial.println(totalUsers);

}
void loadUsers() {
  if (totalUsers >= 1) {
    int sa = USER_DATA_ADDR;
    for (int i = 0; i < totalUsers; i++) {
      user u;

      for (int n = 0; n < ALIAS_LENGTH; n++) {
        u.alias[n] = EEPROM.read(sa);
        sa++;
        // Serial.println(u.alias[n]);
      }

      for (int n = 0; n < MEASUREMENTS; n++) {
        u.measurements[n] = EEPROMReadlong(sa);
        sa += MEASUREMENT_BYTES;
      }
      users[i] = u;
    }
  }

}
void save() {
  if (totalUsers >= 1) {
    EEPROM.write(FIRST_OPEN_ADDR, HAS_BEEN_OPEN);
    EEPROM.write(TOTAL_USERS_ADDR, totalUsers);
    int sa = USER_DATA_ADDR;
    for (int i = 0; i < totalUsers; i++) {
      user u = users[i];

      for (int n = 0; n < ALIAS_LENGTH; n++) {
        EEPROM.write(sa, u.alias[n]);
        sa++;
      }

      for (int n = 0; n < MEASUREMENTS; n++) {
        EEPROMWritelong(sa, u.measurements[n]);
        sa += MEASUREMENT_BYTES;
      }
    }
  }


}

void loop() {
  // put your main code here, to run repeatedly:
  printMenu();

  waitForCommand();

}
void printMenu() {
  Serial.println("");
  Serial.println("Available commands:");
  Serial.println(" m) Main menu");
  switch (currentMenu) {
    case MENU_MAIN:
      Serial.println("Main Menu:");
      Serial.println(" n) New test");
      Serial.println(" u) Users");
      Serial.println(" s) User stats");
      break;
    case MENU_USERS:
      Serial.println("Users:");
      Serial.println(" n) New user");
      Serial.println(" r) Reset all");
      Serial.println(" d) Delete user");
      Serial.println("Select user");
      for (int i = 0; i < totalUsers; i++) {
        String p = String(i + ") ");
        Serial.print(" "); Serial.print(i); Serial.print(") ");
        printUserName(users[i]);
        Serial.print(" best average: ");
        Serial.print(userAverage(users[i]));
        Serial.println(" ms");
      }
      break;
    case MENU_USERS_DELETE:
      Serial.println("Delete a user");
      Serial.println(" c) Cancel");
      for (int i = 0; i < totalUsers; i++) {
        String p = String(i + ") ");
        Serial.print(" "); Serial.print(i); Serial.print(") ");
        printUserName(users[i]);
        Serial.print(" best average: ");
        Serial.print(userAverage(users[i]));
        Serial.println(" ms");
      }
      break;
  }
  Serial.println("");
}
int userAverage(struct user u) {
  long total = 0;
  for (int i = 0; i < MEASUREMENTS; i++) {
    total += u.measurements[i];
  }
  return total / MEASUREMENTS;
}


void waitForCommand() {
  while (!Serial.available());
  char in = Serial.read();
  flushS();
  //global commands
  switch (in) {
    case 'm':
      currentMenu = MENU_MAIN;
      return;
      break;
  }
  if (currentMenu == MENU_MAIN) {
    switch (in) {
      case 'n':
        Serial.println(in);
        newTest();
        return;
        break;
      case 'u':
        currentMenu = MENU_USERS;
        return;
        break;
    }
  }
  if (currentMenu == MENU_USERS) {
    switch (in) {
      case 'n':
        newUser();
        return;
        break;
      case 'r':
        totalUsers = 0;
        newUser();
        return;
        break;
      case 'd':
        currentMenu = MENU_USERS_DELETE;
        return;
        break;
      default:
      
        int nr = String(in).toInt();
        if (nr >= 0 && nr < totalUsers&&isdigit(in)) {
          selectUser(nr);
          return;
        }
        break;

    }
  }
  if (currentMenu == MENU_USERS_DELETE) {
    switch (in) {
      case 'c':
        currentMenu = MENU_USERS;
        return;
        break;
      default:
        int nr = String(in).toInt();
        if (nr >= 0 && nr < totalUsers&&isdigit(in)) {
          deleteUser(nr);
          return;
        }
        break;

    }
  }
  Serial.print("\""); Serial.print(in); Serial.print("\" ");
  Serial.println("is an invalid command");

}
void newUser() {
  if (totalUsers >= MAX_USERS) {
    Serial.println("Too many users please delete one first");
    return;
  }
  user newUser;
  Serial.println("Creating a new user!");
  Serial.println("Plase enter an Alias with a maximum of 10 characters:");
  while (true) {
    while (!Serial.available());
    delay(100);
    if (Serial.available() > ALIAS_LENGTH) {
      Serial.println("Please enter 10 or less characters");
      flushS();
    } else if (Serial.available() > 0) {
      break;
    }
  }
  int charNr = 0;
  while (Serial.available() && charNr < ALIAS_LENGTH) {
    char charIn = Serial.read();
    newUser.alias[charNr] = charIn;
    charNr++;
  }
  while (charNr < ALIAS_LENGTH) {
    newUser.alias[charNr] = ' ';
    charNr++;
  }
  totalUsers++;
  users[totalUsers - 1] = newUser;
  selectUser(totalUsers - 1);
  save();
}

void selectUser(int userNr) {
  currentUser = userNr;
  Serial.print("User ");
  printUserName(users[userNr]);
  Serial.println(" is selected");
}

void deleteUser(int userNr) {
  totalUsers--;
  for (int i = userNr; i < totalUsers; i++)
    users[i] = users[i + 1];
  Serial.print("User ");
  Serial.print(userNr);
  Serial.println(" is deleted");

  if (currentUser > userNr)
    currentUser--;
  else if (currentUser == userNr)
    selectUser(0);
}

void printUserName(struct user u) {
  for (int i = 0; i < ALIAS_LENGTH; i++) {
    Serial.print(u.alias[i]);
  }

}


void  newTest() {
  user tmpu;
  user u = users[currentUser];
  for (int i = 0; i < MEASUREMENTS; i++) {
    measurement(&tmpu.measurements[i]);
  }
  printUserName(u);
  Serial.println(" your results are back!");
  Serial.print(userAverage(tmpu));
  Serial.println(" ms on average");
  if (userAverage(u) > userAverage(tmpu) || userAverage(u) < 100) {
    Serial.println("That is a new record and will be saved");
    for (int i = 0; i < MEASUREMENTS; i++) {
      users[currentUser].measurements[i] = tmpu.measurements[i];
    }
  } else {
    Serial.println("You didn't beat your record though");
  }
  save();
}

void measurement(int * result) {
  Serial.println("New measurement");

  digitalWrite(pinLed, LOW);
  delay(1000 + random(2000));
  digitalWrite(pinLed, HIGH);
  long firstReading = millis();
  while (!digitalRead(pinButton) && millis() - firstReading < 5000);
  digitalWrite(pinLed, LOW);
  long secondReading = millis();
  long dif = secondReading - firstReading;
  Serial.println(dif);
  if (dif >= 1000) {
    Serial.println("Too slow");
    measurement(result);//invalid try
  } else if (dif < 101) { // if faster than the current world record
    Serial.println("Too fast, world record is 101 ms");
    measurement(result);//invalid try
  } else {
    *result = dif;

  }


}
//This function will write a 4 byte (32bit) long to the eeprom at
//the specified address to address + 3.
void EEPROMWritelong(int address, long value)
{
  //Decomposition from a long to 4 bytes by using bitshift.
  //One = Most significant -> Four = Least significant byte
  byte four = (value & 0xFF);
  byte three = ((value >> 8) & 0xFF);
  byte two = ((value >> 16) & 0xFF);
  byte one = ((value >> 24) & 0xFF);

  //Write the 4 bytes into the eeprom memory.
  EEPROM.write(address, four);
  EEPROM.write(address + 1, three);
  EEPROM.write(address + 2, two);
  EEPROM.write(address + 3, one);
}

//This function will return a 4 byte (32bit) long from the eeprom
//at the specified address to address + 3.
long EEPROMReadlong(long address)
{
  //Read the 4 bytes from the eeprom memory.
  long four = EEPROM.read(address);
  long three = EEPROM.read(address + 1);
  long two = EEPROM.read(address + 2);
  long one = EEPROM.read(address + 3);

  //Return the recomposed long by using bitshift.
  return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}
void flushS() {
  delay(100);
  while (Serial.available()) {
    Serial.read();
  }
}
