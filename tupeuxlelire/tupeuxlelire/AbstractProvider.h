#ifndef AbstractProvider_H
#define AbstractProvider_H
#include <string>

using std::string;

class AbstractProvider
{
public:

    // Constructeur/destructeur
    AbstractProvider();
    virtual ~AbstractProvider();

    //Méthodes virtuelles pures: obligation de redéfinition par les héritiers.
    virtual void provide_once() = 0;
    virtual void provide() = 0;

    //Methode virtuelles vides: non-obligation de redéfinition par les héritiers.
    virtual void callbackTest(string reponse);

    //Methodes publiques
    void setFirstDelivery(bool etat);
    bool firstDelivery();
    int recommendedRefreshTime();

protected:
    bool m_firstDelivery;
    int m_recommendedRefreshTime;

    /*
    //Les requètes d'entier
    virtual int query() = 0;
    virtual int queryAll() = 0;

    //Les requètes de QString
    virtual QString query() = 0;
    virtual QString queryAll() = 0;
    */

};

#endif // AbstractProvider_H
