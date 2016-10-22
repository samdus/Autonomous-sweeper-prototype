#ifndef ICONTROLLEUR_PRINCIPAL_H
#define ICONTROLLEUR_PRINCIPAL_H

class IControlleurPrincipal
{
public:
    enum Fonction
    {
        Avance = 1,
        Recule = 2,
        Tourne = 3,
        Stop = 4,
        Orientation = 5,
        Erreur = 6,
        Droite = 7,
        Gauche = 8,
        SetDebug = 9,
        StopDebug = 10,
        ResetErreur = 11,
        InfoVitesseMoteur = 12,
        InfoOrientation = 13,
        InfoDistanceObjet = 14
    };

    enum TypeErreur
    {
        Obstacle = 1,
		FonctionInconnue = 2
    };

    /// \brief Fait avancer le robot pendant la durée donnée
    /// \param dixiemeSec La durée qu'on veut avancer (en dixième de sec)
    /// \return Vrai si ça a fonctionné, faux s'il y a eu un problème
	volatile virtual bool avancePendantXDixiemeSec(int dixiemeSec)volatile = 0;

    /// \brief Fait reculer le robot pendant la durée donnée
    /// \param dixiemeSec La durée qu'on veut reculer (en dixième de sec)
    /// \return Vrai si ça a fonctionné, faux s'il y a eu un problème
	volatile virtual bool reculePendantXDixiemeSec(int dixiemeSec)volatile = 0;

    /// \brief Arrête immédiatement le robot
    /// \return Vrai si ça a fonctionné, faux s'il y a eu un problème
	volatile virtual bool stop()volatile = 0;

    /// \brief Fait tourner le robot jusqu'à l'orientation voulue
    /// \param degres L'orientation désirée (en degré)
    /// \return Vrai si ça a fonctionné, faux s'il y a eu un problème
	volatile virtual bool tourneAuDegresX(int degres)volatile = 0;

    /// \brief Fait tourner le robot vers la gauche du nombre de degré voulue
    /// \param degres Le nombre de degré de la rotation
    /// \return Vrai si ça a fonctionné, faux s'il y a eu un problème
	volatile virtual bool tourneGauche(int degres)volatile = 0;

    /// \brief Fait tourner le robot vers la droite du nombre de degré voulue
    /// \param degres Le nombre de degré de la rotation
    /// \return Vrai si ça a fonctionné, faux s'il y a eu un problème
	volatile virtual bool tourneDroite(int degres)volatile = 0;

    /// \brief Obtient l'orientation actuel
    /// \return L'orientation
	volatile virtual int obtenirOrientation()volatile = 0;

    /// \brief Met en mode debug
    ///        Attention, des données seront reçu 5x/seconde par la suite
	volatile virtual void setDebug()volatile = 0;

    /// \brief Termine le mode debug
	volatile virtual void stopDebug()volatile = 0;

    /// \brief Remet le flag d'erreur à faux
	volatile virtual void resetErreur()volatile = 0;
};

#endif // !ICONTROLLEUR_PRINCIPAL_H
