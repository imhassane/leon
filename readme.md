# TODO

#### Dans le diagramme UML:
- [ ] Représenter l'architecture multi ou mono coeur
- [ ] FT comme option booléenne
- [ ] Le tableau des différences entre les LEON et leur version FT
- [ ] Ecrire les différences entre les LEON


## Leon & Leon FT

#### Différences entre Leon et LeonFT

#### Leon 1
| Leon | Leon FT |
|------|-----|
| | Registres implémentant le modèle TMR c'est-à-dire trois registres en parallèle et un électeur sélectionnant le résultat majoritaire. 

#### Leon 2
La gestion des erreurs est fournie au niveau du design (VHDL), pas besoin d'un processeur qui s'en occupe
| Leon | Leon FT |
|------|-----|
| Des caches d'instructions et de données séparés sont fournis, chacun configurable de 1 à 64 koctets, avec 16-32 octets par ligne  | Le cache d'instructions LEON est un cache mappé directement, configurable de 1 à 64 Ko. Le cache d'instructions est divisé en lignes de cache avec 8 à 32 octets de données. |
| | |


#### Leon 3
|LEON | Leon FT  |
|---|---|
|   | Aucun impact sur les performances avec la détection & correction d'erreurs   |
| LRR & LRU comme caches  | LRU comme unique cache   |
|   | Pas de cache I/D   |
|   | Gestion des erreurs automatique & transparente  |
|   | Register file SEU error-correction of up to 4 errors per 32-bit word |
|   | Cache memory error-correction of up to 4 errors per tag or 32-bit word |
| Avec cache locking | Sans cache locking |

#### Leon 4

# Propriétés de LEON

Liste des propriétés qui sont utilisées pour le processeur LEON.

#### Propriétés LEON
------------------------------------
| Propriété | Valeurs possibles  | Description  |
|---|---|---|
| Frequency | 0 .. 1500 MHz | Fréquence de la plateforme d'exécution (FPGA ou Asic Technologies, ...) |
| IO_Type  | PCI  | Définit le type du bus de transfert |
| IO_Mode | SixtyFourBits (64 Bits), ThirtyTwoBits (32 Bits) | La taille du bus de transfert de données |
| Supported_Fault_Tolerance_Implementation | NoProtection, FourBitsParity, EightBitsParity, SevenBitsBCHRestart, MemoryTriplication, SevenBitsBCHNoRestart, TechnologySpecific | Définit l'algorithme de correction d'érreurs à appliquer |
| Stage_Type | FiveStage [FE, DE, RA, EX, ME, XC, WR], SevenStage [Jumpl, Smul, Umul, Sdiv, Udiv, Atomic_Load, Store] | Définit le nombre d'opérations possibles par l'unité de calcul (FPU) |


#### Propriétés CHEDDAR
