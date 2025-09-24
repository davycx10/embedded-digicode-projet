# Mon projet Digicode avec Raspberry Pi Pico

## Description

Je me suis lancé dans un projet avec mon **Raspberry Pi Pico** pour créer un petit système de digicode. L'idée, c'est de saisir un code via un clavier matriciel. Si le code correspond à celui que j'ai programmé, le système me dit que c'est bon. Sinon, il me dit que c'est rejeté. Et pour ne pas perdre de temps, si je mets plus de 15 secondes à saisir le code, la saisie se réinitialise automatiquement.

### Fonctionnalités principales :
- Saisie d'un code via le clavier matriciel.
- Vérification du code avec un code prédéfini (hardcoded).
- Message de confirmation si le code est correct.
- Message d'erreur si le code est incorrect.
- Réinitialisation de la saisie si aucune touche n'est pressée pendant 15 secondes.

## Matériel requis

- Raspberry Pi Pico.
- Un clavier matriciel (utilisé pour saisir le code).
- Un écran (LCD, OLED, ou via la console) pour afficher les messages.
- Câbles pour connecter le clavier et l'écran.

## Logiciel requis

- **PlatformIO** : outil pour développer et gérer des projets sur des microcontrolleurs.
  - Si tu n'as pas encore installé PlatformIO, tu peux le faire via [VSCode](https://code.visualstudio.com/) (extension PlatformIO) ou installer PlatformIO CLI directement sur ton terminal.

## Installation

### 1. Préparer PlatformIO

Si tu n'as pas encore installé PlatformIO, suis ces étapes :

- Installe [Visual Studio Code (VSCode)](https://code.visualstudio.com/) si ce n'est pas déjà fait.
- Dans VSCode, va dans le menu Extensions et cherche **PlatformIO** pour l'installer.
- Tu peux aussi utiliser la **PlatformIO CLI** si tu préfères travailler en ligne de commande.


