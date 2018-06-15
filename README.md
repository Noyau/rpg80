# RPG80 #

Un projet de RPG en mode console « à l'ancienne », alias L'Arlésienne.

À défaut de faire un vrai LISEZ-MOI, Cahier des charges, Game Doc, etc. Je vais écrire ici ce qui me passe par la tête, on dira que c'est bon, hein.

TRAVAIL EN COURS...

## Structures de données ##

La narration s'articule autour d'une structure en ARBRE : une histoire.

Chaque étape de la narration est un NOEUD de l'ARBRE.

Il y a plusieurs types de NOEUD :

- NARRATION : du texte, éventuellement des choix qui mènent à d'autres NOEUDS
- COMBAT : rencontre avec un ennemi, les sorties peuvent être :
 * VICTOIRE
 * DÉFAITE
 * FUITE
 * INCONSCIENT
 * CAPTURE
- DIALOGUE : rencontre avec personnage, les choix sont multiples et peuvent mener à plusieurs NOEUDS

## Combats ##

Le joueur peut tomber sur des combats (aléatoires ou scénarisés). Le joueur dispose de points de compétences permettant d'influencer les actions qu'il peut faire.
Les actions dépendent aussi de la classe du personnage, son équipement, etc.
Chaque action nécessite de lancer un dé (D2, D6, D20, D100), les résultats peuvent être :
 * RÉUSSITE CRITIQUE
 * RÉUSSITE
 * ÉCHEC
 * ÉCHEC CRITIQUE
Les CRITIQUES auront une influence sur le long terme dans l'histoire.