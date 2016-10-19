#ifndef ICOMMUNICATOR_H
#define ICOMMUNICATOR_H

/// \brief Interface d�finissant les capacit�s de communication avec le Arduino
//         Via le port s�rie
class ICommunicator
{
public:
    /// \brief Fait avancer le robot pendant la dur�e donn�e
    /// \param millisec La dur�e qu'on veut avancer (en ms)
    /// \return Vrai si �a a fonctionn�, faux s'il y a eu un probl�me
    virtual bool avancePendantXMilliSec(int millisec) = 0;

    /// \brief Fait reculer le robot pendant la dur�e donn�e
    /// \param millisec La dur�e qu'on veut reculer (en ms)
    /// \return Vrai si �a a fonctionn�, faux s'il y a eu un probl�me
    virtual bool reculePendantXMilliSec(int millisec) = 0;

    /// \brief Arr�te imm�diatement le robot
    /// \return Vrai si �a a fonctionn�, faux s'il y a eu un probl�me
    virtual bool stop() = 0;

    /// \brief Fait tourner le robot jusqu'� l'orientation voulue
    /// \param degres L'orientation d�sir�e (en degr�)
    /// \return Vrai si �a a fonctionn�, faux s'il y a eu un probl�me
    virtual bool tourneAuDegresX(float degres) = 0;

    /// \brief Obtient l'orientation actuel
    /// \return L'orientation
    virtual float obtenirOrientation() = 0;

    /// \brief D�fini la fonction qui sera appel� lorsque le p�riph�rique
    ///        g�n�rera une interruption. 
    ///        (bloque jusqu'� la r�ception d'un message)
    /// \param fonction La fonction callback 
    ///                 Elle poss�de le param�tre message int[4] qui
    ///                 contiendra jusqu'� 4 messages lues sur le port s�rie
    virtual void setCallBack(void* fonction(int[4])) = 0;
};

#endif // !ICOMMUNICATOR_H
