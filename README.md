# Charbox
api for charbox.org random character generator
# main
```cpp
#include "Charbox.h"
#include <iostream>
#include <locale>
#include <codecvt>

int main() {
   Charbox api;
    auto character = api.get_character("Друид","Круг звёзд","Багбир","Стандартный","Артист",6,"dices",web::json::value::array(),1,"dices","Мужской").then([](json::value result) {
        std::cout << "Search results: " << result.serialize() << std::endl;
    });
    character.wait();
    
    return 0;
}
```

# Launch (your script)
```
g++ -std=c++11 -o main main.cpp -lcpprest -lssl -lcrypto -lpthread -lboost_system -lboost_chrono -lboost_thread
./main
```
