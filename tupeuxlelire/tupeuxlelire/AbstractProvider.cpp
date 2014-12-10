#include "AbstractProvider.h"

// Constructeur/destructeur
AbstractProvider::AbstractProvider()
{
    m_firstDelivery = true;
    m_recommendedRefreshTime = 0;
}

AbstractProvider::~AbstractProvider(){}

// Méthodes
void AbstractProvider::setFirstDelivery(bool etat){
    m_firstDelivery = etat;
}

bool AbstractProvider::firstDelivery(){
    return m_firstDelivery;
}

int AbstractProvider::recommendedRefreshTime(){
    return m_recommendedRefreshTime;
}

void AbstractProvider::callbackTest(string reponse){}

