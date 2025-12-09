# Projet_BUT_S1_Interpreteur_en_C_Gestion-Scolarite
## Objectif du Projet

Ce projet académique consiste à développer un **interpréteur de commande (CLI)** en langage **C** pour simuler le fonctionnement des jurys d'une formation universitaire (BUT Informatique).
Le programme gère les données d'étudiants, calcule les moyennes, et applique les règles de validation et de **compensation** pour les Unités d'Enseignement (UE) et les Regroupements Cohérents d'Unités d'Enseignement (RCUE).

## Fonctionnalités Implémentées

Le programme implémente les commandes suivantes, lues via l'entrée standard (`stdin`):

| Commande | Rôle |
| :--- | :--- |
| `INSCRIRE` | Ajoute un nouvel étudiant. |
| `CURSUS [ID]` | Affiche le statut actuel de l'étudiant (notes, validations, statut global). |
| `NOTE [ID] [Semestre] [UE] [Note]` | Enregistre une nouvelle note et met à jour le statut de validation/compensation. |
| `DEMISSION [ID]` | Modifie le statut de l'étudiant à "Démission". |
| `DEFAILLANCE [ID]` | Modifie le statut de l'étudiant à "Défaillance". |
| `BILAN [Année]` | Effectue un bilan statistique sur la promotion (nombre de démissions, ajournés, etc.). |
| `EXIT` | Quitte le programme. |

## Technologies et Concepts Clés

  * **Langage :** **C** (Impératif).
  * **Algorithmique :** Implémentation de la logique complexe de **compensation** académique entre les UE/RCUE.
  * **Qualité Logicielle :** Utilisation d'**assertions** (`assert()`) pour tester et documenter les préconditions des fonctions (approche par contrat).
  * **Structures de Données :** Utilisation de `struct` (ex: `Eleve`, `Evaluation`) pour modéliser les données de la scolarité.


## Structure du Dépôt

```
.
├── projetp.c                 # Le code source C complet de l'interpréteur.
├── README.md                 # Ce document.
├── in.txt                    # Fichier de commandes d'entrée (tests).
├── inout.txt                 # Fichier de commandes d'entrée et de sortie (tests).

```
