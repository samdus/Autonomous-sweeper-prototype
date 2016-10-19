#ifndef ICOMMUNICATOR_H
#define ICOMMUNICATOR_H

/// \brief Interface définissant les capacités de communication avec le Arduino
//         Via le port série
class ICommunicator
{
public:
    /// \brief Fait avancer le robot pendant la durée donnée
    /// \param millisec La durée qu'on veut avancer (en ms)
    /// \return Vrai si ça a fonctionné, faux s'il y a eu un problème
    virtual bool avancePendantXMilliSec(int millisec) = 0;

    /// \brief Fait reculer le robot pendant la durée donnée
    /// \param millisec La durée qu'on veut reculer (en ms)
    /// \return Vrai si ça a fonctionné, faux s'il y a eu un problème
    virtual bool reculePendantXMilliSec(int millisec) = 0;

    /// \brief Arrête immédiatement le robot
    /// \return Vrai si ça a fonctionné, faux s'il y a eu un problème
    virtual bool stop() = 0;

    /// \brief Fait tourner le robot jusqu'à l'orientation voulue
    /// \param degres L'orientation désirée (en degré)
    /// \return Vrai si ça a fonctionné, faux s'il y a eu un problème
    virtual bool tourneAuDegresX(float degres) = 0;

    /// \brief Obtient l'orientation actuel
    /// \return L'orientation
    virtual float obtenirOrientation() = 0;

    /// \brief Défini la fonction qui sera appelé lorsque le périphérique
    ///        générera une interruption. 
    ///        (bloque jusqu'à la réception d'un message)
    /// \param fonction La fonction callback 
    ///                 Elle possède le paramètre message int[4] qui
    ///                 contiendra jusqu'à 4 messages lues sur le port série
    virtual void setCallBack(void* fonction(int[4])) = 0;
};

#endif // !ICOMMUNICATOR_H
