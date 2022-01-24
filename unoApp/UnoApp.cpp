#include <iostream>
#include "uno/Card.h"

int main()
{
    std::cout << "Hello World\n";
    Card myCard(Card::Red, Card::Reverse);
    myCard.Display();
}
