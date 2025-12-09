#include <stdio.h>
#include <math.h>
#pragma warning(disable: 4996 6031 6054)
#include <string.h>
enum {MAX = 31, MAX_ELEVES = 100, NB_NOTES = 6, NB_S = 6, NB_A = 3, MAX_ADM = 4};
const float NOTE_MOY = 10., NOTE_MAX = 20., NOTE_ELI = 8., NOTE_MIN = 0.;

typedef struct {
    float note[NB_NOTES];
    char adm[NB_NOTES][MAX_ADM]; // Chaque note a son code associé (ex: "ADM", "AJ", "ADS", "ADC")
} Evaluation;

typedef struct {
    char prenom[MAX], nom[MAX], valid[MAX];
    int code, semestre, but;  //code est le numéros d'étudiant
    Evaluation S[NB_S];  // Semestres
    Evaluation B[NB_A];  // Bilans
} Eleve;

// Inscrit un élève en enregistrant son prénom, nom et identifiant et renvoyant le nombre d'élève mis à jour
unsigned int inscrire(unsigned int c, Eleve *eleves) {
    Eleve e;
    scanf("%s%s", e.prenom, e.nom);
    strcpy(e.valid, " en cours");
    e.semestre = 0, e.but=0;
    // Vérification si doublon
    for(int i = 0; i < c; ++i) {
        if(strcmp(eleves[i].prenom, e.prenom) == 0 && strcmp(eleves[i].nom, e.nom) == 0) {
            printf("Nom incorrect\n");
            return c;
        }
    }
    // Enregistrement du code de l'élève
    e.code = c+1;
    // Initialisation des notes
    for(int j=0; j<NB_S; ++j) {
        for(int i=0; i<NB_NOTES; ++i) {
            e.S[j].note[i] = MAX;
        }
    }
    eleves[c] = e;
    printf("Inscription enregistree (%u)\n", e.code);
    ++c;
    return c;
}

// Vérification si l'étudiant est toujours en formation retourne 1 si la personne est en formation sinon 0
unsigned int verif(const Eleve *eleves) {
    if(strcmp(eleves->valid, " defaillance") == 0 || strcmp(eleves->valid, " demission") == 0 || strcmp(eleves->valid, " ajourne") == 0) {
        return 0;
    }
    return 1;
}

// Vérification si l'id correspond à un étudiants enregistré si oui retourne 1 sinon 0
unsigned int verifId(unsigned int id, unsigned int nbId){
    if(id>nbId || id<0){
        printf("Identifiant incorrect\n");
        return 0;
    }
    return 1;
}

// Montre les notes et leur état
void cursus(const Eleve *eleves) {
    printf("%u %s %s", eleves->code, eleves->prenom, eleves->nom);
    for(int j=0; j<=eleves->semestre && j<NB_S; ++j) {
        printf("\nS%u -", j + 1);
        //Affiche la ligne du semestre
        for (int i=0; i<NB_NOTES; ++i) {
            if (eleves->S[j].note[i] == MAX)
                printf(" * (*) -");
            else
                printf(" %.1f (%s) -", floorf(eleves->S[j].note[i]*10.f)/10.f, eleves->S[j].adm[i]); 
        }
        //Affiche la ligne de l'année
        if(j!=0 && eleves->but>j/2. && (j+1)%2==0) {
            printf("\nB%u -", (j+1)/2);
            for (int i=0; i<NB_NOTES; ++i)
                printf(" %.1f (%s) -", floorf(eleves->B[(j+1)/2-1].note[i]*10.f)/10.f, eleves->B[(j+1)/2-1].adm[i]);
        }
    }
    printf("%s\n", eleves->valid);
    return;
}

// Enregistre les notes
void note(Eleve *eleves, unsigned int nbId) {
    unsigned int ue;
    float note;
    scanf("%u %f", &ue, &note);
    // Vérifications si l'étudiant est toujours en formation, si la note est correct et si l'ue est valide
    if(verif(eleves) == 0) {
        printf("Etudiant hors formation\n");
        return;
    }
    if(note<NOTE_MIN || note>NOTE_MAX) {
        printf("Note incorrecte\n");
        return;
    }
    if (ue<1 || ue>NB_NOTES) {
        printf("UE invalide\n");
        return;
    }
    // Enregistrement des admission
    eleves->S[eleves->semestre].note[ue-1] = note;
    printf("Note enregistree\n");
    if (note<NOTE_MOY)
        strcpy(eleves->S[eleves->semestre].adm[ue-1], "AJ");
    else
        strcpy(eleves->S[eleves->semestre].adm[ue-1], "ADM");
    return;
}

// Met à jour l'état administratif de l'élève pour démission
void demission(Eleve *eleves) {
    // Vérification si l'étudiant est toujours en formation
    if(verif(eleves) == 0) {
        printf("Etudiant hors formation\n");
        return;
    }
    strcpy(eleves->valid, " demission");
    printf("Demission enregistree\n");
}

// Met à jour l'état administratif de l'élève pour défaillance
void defaillance(Eleve *eleves) {
    // Vérification si l'étudiant est toujours en formation
    if(verif(eleves) == 0) {
        printf("Etudiant hors formation\n");
        return;
    }
    strcpy(eleves->valid, " defaillance");
    printf("Defaillance enregistree\n");
}

// Calcule le nombre d'étudiants dans un semestre précis et donc renvoie ce nombre
unsigned int nbEtu(const Eleve *eleves, unsigned int semestre, unsigned int c) {
    unsigned int nb = 0;
    for(int i=0; i<c; ++i) {
        if(eleves[i].semestre == semestre && verif(&eleves[i]))
            ++nb;
    }
    return nb;
}

// Met à jour l'état administratif de l'élève pour diplomé ou ajourné pour la dernière année 
void diplome(Eleve *eleves) {
    for(int i=0; i<NB_NOTES; ++i) {
        //Vérifie si une note est < 10 dans le dernier bilan si oui met à jour pour ajourné
        if(eleves->B[NB_A-1].note[i]<NOTE_MOY) {
            strcpy(eleves->valid, " ajourne");
            return;
        }
    }
    //Après verification met à jour pour diplomé
    strcpy(eleves->valid, " diplome");
}

// Effectue le bilan d'une année scolaire (semestre paire) pour un élève retourne 1 si l'élève peut passer, 0 s'il est ajourné
unsigned int bilanAnn(Eleve *eleves) {
    unsigned nbAJ=0, nbAJB=0;
    int semestre=eleves->semestre;
    // Calcul de la moyenne annuelle pour chaque UE
    for(int i=0; i<NB_NOTES; ++i) {
        eleves->B[(semestre-1)/2].note[i] = (eleves->S[semestre].note[i]+eleves->S[semestre-1].note[i])/2.0;
        // Cas 1 : note < 8 enregistre "AJB"
        if(eleves->B[(semestre-1)/2].note[i]<NOTE_ELI) {
            strcpy(eleves->B[(semestre-1)/2].adm[i], "AJB");
            ++nbAJB;
        } 
        // Cas 2 : note >= 10 enregistre "ADM"
        else if(eleves->B[(semestre-1)/2].note[i] >= NOTE_MOY){
            strcpy(eleves->B[(semestre-1)/2].adm[i], "ADM");
            // Vérifie les bilans précédents : enregistre "ADS" si compensé rétroactivement
            for (int k=0; k<((semestre-1)/2); ++k) {
                if (eleves->B[k].note[i] < NOTE_MOY) {
                    strcpy(eleves->B[k].adm[i], "ADS");
                    // Mise à jour également au niveau des semestres
                    if (eleves->S[k*2].note[i] < NOTE_MOY)
                        strcpy(eleves->S[k*2].adm[i], "ADS");
                    if (eleves->S[k*2+1].note[i] < NOTE_MOY)
                        strcpy(eleves->S[k*2+1].adm[i], "ADS");
                }
            }
             // Cas particulier : Si note de semestre < 10 mais bilan >=10 enregistre "ADC"
            if(eleves->S[semestre].note[i] < NOTE_MOY)
                strcpy(eleves->S[semestre].adm[i], "ADC");
            else if (eleves->S[semestre-1].note[i] < NOTE_MOY)
                strcpy(eleves->S[semestre-1].adm[i], "ADC");
        }
        // Cas 2 : note >= 8 mais < 10 enregistre "AJ"
        else {
            strcpy(eleves->B[(semestre-1)/2].adm[i], "AJ");
            ++nbAJ;
        } 
    }
    for (int i = 0; i < NB_NOTES; ++i) {
         if (eleves->B[(semestre-1)/2].note[i] >= NOTE_ELI && eleves->B[(semestre-1)/2].note[i] < NOTE_MOY) {
            // Vérifier bilan précédent s'il existe
            if ((semestre-1)/2 > 0) {
                float notePrec = eleves->B[(semestre-1)/2 - 1].note[i];
                if (notePrec >= NOTE_ELI && notePrec < NOTE_MOY) {
                    strcpy(eleves->valid, " ajourne");
                    ++eleves->but;
                    return 0;
                }
            }
        }
    }
    // Si l'élève a au moins une note < 8 ou plus de deux notes entre 8 et 10 enregistre ajourné (état administratif)
    if (nbAJB!=0 || nbAJ>2) {
        strcpy(eleves->valid, " ajourne");
        ++eleves->but;
        return 0;
    }
    //Sinon passe à l'année suivante
    ++eleves->but;
    return 1;
}


// Valide ou non le semestre d'un ensemble d'élèves
void jury(Eleve *eleves, unsigned int semestre, unsigned int c) {
    if(semestre>NB_S-1 || semestre<0) {
        printf("Semestre incorrect\n");
        return;
    }
    //Nombre d'étudiants concernés
    unsigned int nb=nbEtu(eleves, semestre, c);
    // Vérification que toutes les notes ont été enregistrées
    for(int i=0; i<c; ++i) {
        if ((eleves[i].semestre == semestre) && (verif(&eleves[i]))) {
            for (int j=0; j<NB_NOTES; ++j) {
                if (eleves[i].S[semestre].note[j] == MAX) {
                    printf("Des notes sont manquantes\n");
                    return;
                }
            }
        }
    }
    // Pour chaque élève, mise à jour du semestre ou décision de fin
    for(int i=0; i<c; ++i) {
        if (verif(&eleves[i]) && (eleves[i].semestre == semestre)) {
            // Si c’est la fin d’une année (semestre pair)
            if ((semestre+1)%2 == 0) {
                if (bilanAnn(&eleves[i]))
                    // Dernière année, diplome si les conditions sont respectées
                    if (semestre == NB_S-1)
                        diplome(&eleves[i]);
                    else
                        ++eleves[i].semestre; //Passe à l'année suivante
            }
            // Si c’est un semestre impair, passe automatiquement au semestre suivant
            else
                ++eleves[i].semestre;
        }
    }
    printf("Semestre termine pour %u etudiant(s)\n", nb);
}

// Affiche les étudiants avec leur informations
void etudiants(const Eleve* eleves, unsigned c) {
    for(int i=0; i<c; ++i)
        printf("%u - %s %s - S%u -%s\n", eleves[i].code , eleves[i].prenom, eleves[i].nom, eleves[i].semestre + 1, eleves[i].valid);
}

// Affiche le nombre d'étudiants dans chaque état administratif pour une année précise
void bilan(const Eleve *eleves, unsigned int annee, unsigned int c) {
    unsigned int dem=0, def=0, cou=0, aj=0, pas=0;
    //Vérifaation de l'année
    if(annee>3){
        printf("Annee incorrecte\n");
        return;
    }
    //Calcul le nombre de personnes dans chaque cas
    for(int id=0; id<c; ++id) {
        if((strcmp(eleves[id].valid, " demission") == 0) && ((annee-1)*2==(eleves[id].semestre) ||  (annee-1)*2+1==(eleves[id].semestre)))
            ++dem;
        else if((strcmp(eleves[id].valid, " ajourne") == 0)&& ((annee-1)*2==(eleves[id].semestre) ||  (annee-1)*2+1==(eleves[id].semestre )))
            ++aj;
        else if((strcmp(eleves[id].valid, " en cours") == 0) && (annee >= (eleves[id].semestre + 1.) / 2.))
            ++cou;
        else if((strcmp(eleves[id].valid, " defaillance") == 0)&& ((annee-1)*2==(eleves[id].semestre) ||  (annee-1)*2+1==(eleves[id].semestre)))
            ++def;
        else if((strcmp(eleves[id].valid, " diplome") == 0) || annee*2<=(eleves[id].semestre))
            ++pas;
    }
    printf("%u demission(s)\n%u defaillance(s)\n%u en cours\n%u ajourne(s)\n%u passe(s)\n", dem, def, cou, aj, pas);
}

// Main
int main() {
    char commande[MAX];
    Eleve eleves[MAX_ELEVES];
    unsigned int c = 0, id, s, an;
    while (1) {
        scanf("%s", commande);
        if(strcmp(commande, "EXIT") == 0)
            break;
        else if(strcmp(commande, "INSCRIRE") == 0) {
            c=inscrire(c, eleves);
        }
        else if(strcmp(commande, "CURSUS") == 0) {
            scanf("%u", &id);
            if(verifId(id,c))
                cursus(&eleves[id-1]);
        }
        else if(strcmp(commande, "NOTE") == 0) {
            scanf("%u", &id);
            if(verifId(id,c))
                note(&eleves[id-1], c);
        }
        else if(strcmp(commande, "DEMISSION") == 0) {
            scanf("%u", &id);
            if(verifId(id,c))
                demission(&eleves[id-1]);
        }
        else if(strcmp(commande, "DEFAILLANCE") == 0) {
            scanf("%u", &id);
            if(verifId(id,c))
                defaillance(&eleves[id-1]);
        }
        else if(strcmp(commande, "JURY") == 0) {
            scanf("%u", &s);
            jury(eleves, s-1, c);
        }
        else if(strcmp(commande, "ETUDIANTS") == 0)
            etudiants(eleves, c);

        else if(strcmp(commande, "BILAN") == 0) {
            scanf("%u", &an);
            bilan(eleves, an, c);
        }
    }
}
