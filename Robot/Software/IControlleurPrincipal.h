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
		DroitePendant = 9,
		GauchePendant = 10,
        SetDebug = 11,
        StopDebug = 12,
        ResetErreur = 13,
        InfoVitesseMoteur = 14,
        InfoOrientation = 15,
        InfoDistanceObjet = 16,
		RetourBool = 17,
		RetourInt = 18 
    };

    enum TypeErreur
    {
        Obstacle = 1,
		FonctionInconnue = 2,
		IO = 3
    };

    /// \brief Fait avancer le robot pendant la dur�e donn�e
    /// \param dixiemeSec La dur�e qu'on veut avancer (en dixi�me de sec)
    /// \return Vrai si �a a fonctionn�, faux s'il y a eu un probl�me
	 virtual bool avancePendantXDixiemeSec(int dixiemeSec) = 0;

    /// \brief Fait reculer le robot pendant la dur�e donn�e
    /// \param dixiemeSec La dur�e qu'on veut reculer (en dixi�me de sec)
    /// \return Vrai si �a a fonctionn�, faux s'il y a eu un probl�me
	 virtual bool reculePendantXDixiemeSec(int dixiemeSec) = 0;

    /// \brief Fait tourner le robot jusqu'� l'orientation voulue
    /// \param degres L'orientation d�sir�e (en degr�)
    /// \return Vrai si �a a fonctionn�, faux s'il y a eu un probl�me
	 virtual bool tourneAuDegresX(int degres) = 0;

    /// \brief Fait tourner le robot vers la gauche du nombre de degr� voulue
    /// \param degres Le nombre de degr� de la rotation
    /// \return Vrai si �a a fonctionn�, faux s'il y a eu un probl�me
	 virtual bool tourneGauche(int degres) = 0;

    /// \brief Fait tourner le robot vers la droite du nombre de degr� voulue
    /// \param degres Le nombre de degr� de la rotation
    /// \return Vrai si �a a fonctionn�, faux s'il y a eu un probl�me
	 virtual bool tourneDroite(int degres) = 0;

	 /// \brief Fait tourner le robot vers la gauche pendant le nombre de dixi�me de seconde voulue
	 /// \param dixiemeSec La dur�e qu'on veut tourner (en dixi�me de sec)
	 /// \return Vrai si �a a fonctionn�, faux s'il y a eu un probl�me
	 virtual bool tourneGauchePendant(int dixiemeSec) = 0;

	 /// \brief Fait tourner le robot vers la droite pendant le nombre de dixi�me de seconde voulue
	 /// \param dixiemeSec La dur�e qu'on veut tourner (en dixi�me de sec)
	 /// \return Vrai si �a a fonctionn�, faux s'il y a eu un probl�me
	 virtual bool tourneDroitePendant(int dixiemeSec) = 0;

    /// \brief Obtient l'orientation actuel
    /// \return L'orientation
	 virtual int obtenirOrientation() = 0;

    /// \brief Met en mode debug
    ///        Attention, des donn�es seront re�u 5x/seconde par la suite
	 virtual void setDebug() = 0;

    /// \brief Termine le mode debug
	 virtual void stopDebug() = 0;

    /// \brief Remet le flag d'erreur � faux
	 virtual void resetErreur() = 0;
};

#endif // !ICONTROLLEUR_PRINCIPAL_H
