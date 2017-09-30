Moteurs de jeux : compte-rendu de TP1
=====================================


Question 1
----------

MainWidget hérite de la classe QOpenGLWidget. Elle contient la boucle de rendu 
et la gestion d'événements utilisateur.

GeometryEngine définit la géométrie à afficher, et la routine d'affichage de
celle-ci.

Les fichiers vshader.glsl et fshader.glsl sont respectivement les vertex et 
fragment shaders utilisés pour dessiner la géométrie. Le vertex shader 
transforme simplement les model coordinates des sommets en screen coordinates 
via la matrice de model-view-projection définie en uniform. Pour chaque pixel
de l'image finale, le fragment shader y attribue la couleur du texel associé.

On renommera les deux fichiers en geom_textured.vert et geom_textured.frag pour
plus de clarté.


Question 2
----------

La classe GeometryEngine utilise des classes wrappers de Qt autour des
fonctions et concepts OpenGL, pour définir la géométrie à dessiner et pour la 
dessiner.

La méthode initCubeGeometry() définit un maillage indexé et texturé, soit un
cube de :
- 24 sommets, soit quatre faces.
- 34 indices de sommets.
- 24 coordonnées de texture.

Les sommets et coordonnées de texture sont stockées dans un tableau de manière
entrelacée ; les indices dans un autre tableau.

La méthode drawCubeGeometry() définit la manière dont les données du maillage
sont transmises aux shaders et dessine le maillage à l'écran. La partie 
"Comment la dessiner" est laissée au QOpenGLShaderProgram passé en argument de 
la méthode.


Question 3
----------

L'algorithme calcule les positions des sommets et les indices en une double 
boucle. Les coordonnées X et Z sont définies dans l'ensemble 
\[0, tailleTerrain\]. La hauteur d'un sommet est définie par un générateur 
pseudo-aléatoire C++11.

Pour garder un effet plus naturel, les sommets situées au frontières du terrain 
ont une hauteur nulle.

On pourrait réduire le transfert de données au GPU en ne définissant qu'une 
hauteur pour chaque sommet, les coordonnées X et Z étant calculées dans le 
vertex shader.


Question 4
----------

J'ai supprimé la logique de rotation de la caméra de l'exemple. S'il y a besoin 
d'une rotation de caméra, je l'implémenterai.

Afin de déplacer la caméra avec les touches directionnelles, j'ai ajouté des 
attributs à la classe MainWidget : il s'agit des données strictement 
nécessaires à l'implémentation d'une caméra, soit trois vecteurs décrivant 
respectivement la position de la caméra, sa cible ainsi que son vecteur up.

La matrice de vue est réinitialisée avec un lookat, avec ces trois valeurs, 
dans la méthode paintGL().

Ensuite, j'ai implémenté une classe CameraController, contenant la logique 
d'interaction avec l'utilisateur. Elle agit comme un event filter pour la 
classe MainWidget. Cela permet de séparer les responsabilités et sera plus 
facile à réutiliser pour les prochains TP et le projet. CameraController se 
charge d'intercepter les événements clavier de MainWidget et de de calculer 
pour elle les nouvelles données de caméra. MainWidget les utilise pour composer 
la matrice de vue.

Problème rencontré pour un cas similaire l'année dernière : une boucle de rendu 
est différente d'une boucle d'événements d'interface graphique. Il ne faut donc
 pas que la caméra réagisse directement aux événements clavier. On préférera 
 activer ou désactiver des flags reflétant ces changements. La matrice de vue 
 sera recalculée lors de la prochaine itération de la boucle de rendu.

Il sera également utile de modifier la boucle de rendu afin d'estimer le temps 
passé depuis la dernière frame.

Déplacer la caméra avec une touche clavier se déroule comme qui suit :
- l'utilisateur presse ou relâche une touche
- le flag associé est activé ou désactivé
- à l'itération de rendu suivante, la direction dans laquelle la caméra doit se 
déplacer est calculée selon les flags activés. Les touches Haut et Bas 
déplaceront l caméra sur son axe Z ; les touches Gauche et Droite sur son 
axe X.
- Les nouveaux vecteurs eye et target de la caméra sont calculés en fonction de 
cette direction.

Pour déplacer la caméra sur ses axes X et Z, il faut exploiter les vecteurs 
eye, target et up. Calculer le vecteur de vue à partir des vecteurs eye et 
target ; calculer le vecteur droit qui est le produit vectoriel des vecteurs 
vue et up.


Bonus
-----




