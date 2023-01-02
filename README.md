# Projet TIV 4INFO 2022-2023 - Option MI
### EVENO Amélie - HUMEAU Dorian - ROZELAAR Marceline - SOREL Maxime

## Chaîne de traitement

1. **Redressement de l'image**  
> On détecte les croix situées dans les coins inférieur gauche et supérieur droit de l'image.
A partir des coordonnées de ces points, on peut d'abord procéder à une translation qui permet d'aligner le point central de la croix inférieure gauche sur la diagonale. On procède ensuite à une rotation pour aligner le point central de la croix supérieure droite sur la diagonale également.  
Cette partie est gérée par le module ```imageRectifier```.


2. **Reconnaissance des modèles de symboles**  
> On réalise ensuite un template matching sur l'image qui permet de récupérer les positions des modèles de symboles.  
Cette partie est gérée par le module ```extractLines```. Nous avons essayé deux méthodes différentes lors du template matching, utilisant chacune un système de calcul complètement différent : ```TM_CCORR_NORMED``` et ```TM_CCOEFF_NORMED```. Les résultats des deux méthodes sont détaillés plus loin dans l'analyse de nos performances.


3. **Extraction de la ligne de symboles et de la taille si spécifiée**  
> A partir des coordonnées récupérées pour chaque modèle de symbole, on peut extraire une partie de l'image rognée sous forme de rectangle. On récupère ainsi la ligne de carrés où les symboles sont dessinés à la main par le scripteur.  
Un deuxième template matching est réalisé sur la ligne afin de repérer l'éventuelle inscription spécifiant la taille. Le résultat est stocké dans une chaîne de caractères ("small", "medium", "large" ou vide) et passé comme paramètre à la suite de la chaîne de traitement pour la future écriture du fichier texte labellisant les imagettes.  
Cette partie est également gérée par le module ```extractLines```.


4. **Extraction des carrés d'imagettes de la ligne**
> Sur l'image rectangulaire contenant la ligne de symboles (modèle et dessins dans les cases carrées), on applique d'abord un algorithme de détection de carrés et rectangles. Cet algorithme utilise la fonction ```findContours()``` d'OpenCV et réalise plusieurs tests successifs pour déterminer s'il s'agit d'un carré. Ces tests vérifient qu'il y a bien 4 sommets au contour et que les angles sont des angles droits.  
On filtre ensuite le résultat de cet algorithme en deux temps.  
L'application d'un seuil de taille permet d'abord de ne pas tenir compte des carrés et rectangles qui ne correspondent pas à des cases mais aux contours du grand rectangle de la ligne ou à un plus petit carré faisant partie du modèle / dessin. L'application d'un seuil de distance entre les centres des carrés deux à deux permet d'éliminer les carrés "dupliqués" qui se trouvent quasiment à la même position.  
Ensuite, il ne reste normalement plus que 5 carrés dont les coordonnées permettent d'extraire les 5 imagettes qui nous intéressent par rognage.  
Cette partie est gérée par le module ```extract_thumbnails```.  


5. **Génération des fichiers pour labellisation**  
> Les matrices représentant les imagettes récupérées sont ensuite utilisées pour la création des fichiers image en .png qui nous intéressent. Ces fichiers sont nommés d'après un préfixe déterminé précédemment dans la chaîne de traitement et au numéro de colonne.  
Le fichier texte en .txt est nommé de la même façon et contient les informations suivantes : label, numéro de formulaire, numéro de scripteur, numéro de page, ligne, colonne et taille (si spécifiée). Toutes les informations sont déduisibles du nom du fichier sauf la taille qui est renseignée d'après le résultat récupéré lors de l'étape 3.  
> Cette partie est également gérée par le module ```extract_thumbnails``` et les fichiers générés sont stockés dans le dossier ```output```.  


# Performances

## Cas limites
Les cas limites que nous avons identifiés sont les suivants :
- certaines pages présentent des défauts au niveau des croix dans les coins qui nous servent de repères pour redresser l'image, notamment des traces de stylo ou blanco. Une succession d'opérations d'érosion et dilatation au cours de la première étape de notre chaîne de traitement permet de détecter malgré tout les croix dans la plupart des cas.
- certaines pages ne sont pas des formulaires à proprement parler mais contiennent uniquement du texte écrit à la main qui ne nous intéresse pas (ex : formulaire n°00022 dans la base d'entraînement).
- si un scripteur dessinait un carré parfait d'une taille proche des cases, cela pourrait poser problème à l'extraction des imagettes, mais nous n'avons pas rencontré ce cas.

## Taux de succès et qualité de l'extraction

Sur la base d'entraînement, 755/770 pages sont correctement rectifiées (si une croix n'est pas détectée, la page est adandonnée, et les pages avec le texte manuscrit a été exclu)
Cela représente alors 98% de réussite.

Sur la base de test, seulement 8/12 pages sont correctement rectifiées.

### Avec CCORR (corrélation croisée)

Les données de sortie comptent 44372 fichiers (imagettes + fichier texte, soit 22186 imagettes)
En théorie, avec nos 755 pages on devrait avoir 26425 imagettes, cela représente donc 83,96% de réussite sur cette étape.

Le taux de réussite global est alors de 82,32% (22186/(770*35))

Avec la méthode par corrélation croisée, on a en sortie 448 fichiers (soit 224 imagettes) sur la base de test.

Le taux de réussite global est alors de 53,33%.
De plus, on a trouvé 5 imagettes qui n'ont pas été labellisées correctement (à chaque fois, le logo "accident" a été détecté au lieu de "person").

Cela s'explique car les images données sur le jeu de test n'ont pas du tout la même luminosité et ainsi, 
les niveaux de gris sont très différents, ce qui joue énormément sur la méthode par corrélation croisée 
qui va justement mesurer là où la luminosité est la plus proche en tous points de l'image.

### Avec CCOEFF (coefficient de corrélation)

Les données de sortie comptent 44356 fichiers (soit 22178 imagettes)
En théorie, avec nos 755 pages on devrait avoir 26425 imagettes, cela représente donc XX,XX% de réussite sur cette étape.

Le taux de réussite global est alors de 82,29% (22178/(770*35))

Avec la méthode par coefficient de corrélation, on a en sortie 548 fichiers (soit 274 imagettes) sur la base de test, ce qui correspond à un taux de réussite de 97.85% si on ne compte que les 8 pages extraites.

Le taux de réussite global est alors de 65,24%. (avec les 12 pages, 4 ayant été écartées lors de la première étape).
Aucune image est fausse-positive : chaque image a bien été répertoriée lors de cette méthode.

Cette méthode, utilisant un niveau binaire et non les niveaux de gris, 
est bien plus précise lorsque l'image ne possède pas la même luminosité car on n'enlève la contrainte de niveau de gris.
Elle est donc bien plus polyvalente en général.
