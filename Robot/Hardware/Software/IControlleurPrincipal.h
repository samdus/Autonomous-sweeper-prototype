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
	 virtual bool avancePendantXDixiemeSec(int dixiemeSec) = 0;

    /// \brief Fait reculer le robot pendant la durée donnée
    /// \param dixiemeSec La durée qu'on veut reculer (en dixième de sec)
    /// \return Vrai si ça a fonctionné, faux s'il y a eu un problème
	 virtual bool reculePendantXDixiemeSec(int dixiemeSec) = 0;

    /// \brief Arrête immédiatement le robot
    /// \return Vrai si ça a fonctionné, faux s'il y a eu un problème
	 virtual bool stop() = 0;

    /// \brief Fait tourner le robot jusqu'à l'orientation voulue
    /// \param degres L'orientation désirée (en degré)
    /// \return Vrai si ça a fonctionné, faux s'il y a eu un problème
	 virtual bool tourneAuDegresX(int degres) = 0;

    /// \brief Fait tourner le robot vers la gauche du nombre de degré voulue
    /// \param degres Le nombre de degré de la rotation
    /// \return Vrai si ça a fonctionné, faux s'il y a eu un problème
	 virtual bool tourneGauche(int degres) = 0;

    /// \brief Fait tourner le robot vers la droite du nombre de degré voulue
    /// \param degres Le nombre de degré de la rotation
    /// \return Vrai si ça a fonctionné, faux s'il y a eu un problème
	 virtual bool tourneDroite(int degres) = 0;

    /// \brief Obtient l'orientation actuel
    /// \return L'orientation
	 virtual int obtenirOrientation() = 0;

    /// \brief Met en mode debug
    ///        Attention, des données seront reçu 5x/seconde par la suite
	 virtual void setDebug() = 0;

    /// \brief Termine le mode debug
	 virtual void stopDebug() = 0;

    /// \brief Remet le flag d'erreur à faux
	 virtual void resetErreur() = 0;
};

#endif // !ICONTROLLEUR_PRINCIPAL_H
