WebApp du robot
================

Utilisation
---------------
Installer nodejs
https://nodejs.org/en/

Installer meteor 
https://www.meteor.com/install

Aller en ligne de commande dans le dossier du projet et exécuter
meteor npm install

Pour démarrer le projet :
meteor 

Pour la console de base de donnée
meteor mongo

Pour la console meteor
meteor shell


Hosting
---------------
L'application est hoster sur (2 cent par heure par container, l'app est shutdown quand on en a pas de besoin):
http://gsdproject.meteorapp.com/

La base de donnée (500mb free)
https://mlab.com/home


Pour déployer :
meteor deploy --settings settings.json gsdproject.meteorapp.com



