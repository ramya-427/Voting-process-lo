#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CANDIDATES 3
#define MAX_NAME_LENGTH 50
#define RESULT_FILE "voting_results.txt"
#define MAX_VOTERS 3

typedef struct Voter {
    char name[MAX_NAME_LENGTH];
    int id;
    int verified;
    int votedFor;
    struct Voter* next;
} Voter;

typedef struct Queue {
    Voter* front;
    Voter* rear;
} Queue;

int candidateVotes[MAX_CANDIDATES] = {0};
char* candidateNames[MAX_CANDIDATES] = {"Candidate A", "Candidate B", "Candidate C"};
Voter* voterList[MAX_VOTERS];
int voterCount = 0;
char* votersForCandidates[MAX_CANDIDATES][MAX_VOTERS] = {{NULL}, {NULL}, {NULL}}; // Tracking voters for each candidate

void initQueue(Queue* q) {
    q->front = q->rear = NULL;
}

Voter* createVoter(char name[], int id, int verified) {
    Voter* newVoter = (Voter*)malloc(sizeof(Voter));
    strcpy(newVoter->name, name);
    newVoter->id = id;
    newVoter->verified = verified;
    newVoter->votedFor = -1; // -1 means not voted
    newVoter->next = NULL;
    if (voterCount < MAX_VOTERS) {
        voterList[voterCount++] = newVoter;
    }
    return newVoter;
}

void enqueue(Queue* q, Voter* voter) {
    if (!q->rear) {
        q->front = q->rear = voter;
    } else {
        q->rear->next = voter;
        q->rear = voter;
    }
}

Voter* dequeue(Queue* q) {
    if (!q->front) return NULL;
    Voter* temp = q->front;
    q->front = q->front->next;
    if (!q->front) q->rear = NULL;
    return temp;
}

void startVoting(Queue* q) {
    printf("\n--- Voting Started ---\n");
    while (q->front) {
        Voter* voter = dequeue(q);
        printf("\nVoter Name: %s\n", voter->name);
        if (voter->verified) {
            printf("Select your candidate:\n");
            for (int i = 0; i < MAX_CANDIDATES; i++) {
                printf("%d. %s\n", i + 1, candidateNames[i]);
            }
            int choice;
            do {
                printf("Enter your choice (1-%d): ", MAX_CANDIDATES);
                scanf("%d", &choice);
            } while (choice < 1 || choice > MAX_CANDIDATES);
            candidateVotes[choice - 1]++;
            voter->votedFor = choice - 1; // Track the voted candidate

            // Add the voter to the list of voters for that candidate
            for (int i = 0; i < MAX_VOTERS; i++) {
                if (votersForCandidates[choice - 1][i] == NULL) {
                    votersForCandidates[choice - 1][i] = voter->name;
                    break;
                }
            }

            printf("Thank you for voting!\n");
        } else {
            printf("You are not verified to vote.\n");
        }
    }
}

void displayResults() {
    printf("\n--- Voting Results ---\n");
    int maxVotes = 0;
    int winningCandidate = -1;

    for (int i = 0; i < MAX_CANDIDATES; i++) {
        printf("%s: %d votes\n", candidateNames[i], candidateVotes[i]);
        if (candidateVotes[i] > maxVotes) {
            maxVotes = candidateVotes[i];
            winningCandidate = i;
        }
    }

    if (winningCandidate != -1) {
        printf("\nWinner: %s with %d votes\n", candidateNames[winningCandidate], maxVotes);
        printf("Members who voted for %s:\n", candidateNames[winningCandidate]);
        for (int i = 0; i < MAX_VOTERS; i++) {
            if (votersForCandidates[winningCandidate][i] != NULL) {
                printf("%s\n", votersForCandidates[winningCandidate][i]);
            }
        }
    } else {
        printf("No winner yet.\n");
    }
}

void saveResultsToFile() {
    FILE* file = fopen(RESULT_FILE, "w");
    if (!file) {
        printf("Error saving results to file.\n");
        return;
    }
    fprintf(file, "--- Voting Results ---\n");
    for (int i = 0; i < MAX_CANDIDATES; i++) {
        fprintf(file, "%s: %d votes\n", candidateNames[i], candidateVotes[i]);
    }
    fclose(file);
    printf("Results saved to %s\n", RESULT_FILE);
}

void resetVotes() {
    for (int i = 0; i < MAX_CANDIDATES; i++) {
        candidateVotes[i] = 0;
    }
    printf("All votes reset.\n");
}

void adminMenu() {
    int choice;
    do {
        printf("\n--- Admin Menu ---\n");
        printf("1. View Voter List\n");
        printf("2. Reset Votes\n");
        printf("3. Save Results to File\n");
        printf("4. Exit Admin Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                printf("\n--- Voter List ---\n");
                for (int i = 0; i < MAX_VOTERS; i++) {
                    printf("ID: %d, Name: %s, Verified: %s\n",
                           voterList[i]->id,
                           voterList[i]->name,
                           voterList[i]->verified ? "Yes" : "No");
                }
                break;
            case 2:
                resetVotes();
                break;
            case 3:
                saveResultsToFile();
                break;
            case 4:
                printf("Exiting Admin Menu.\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (choice != 4);
}

int main() {
    Queue votingQueue;
    initQueue(&votingQueue);

    enqueue(&votingQueue, createVoter("Neha", 1, 1));
    enqueue(&votingQueue, createVoter("Ramya", 2, 1));
    enqueue(&votingQueue, createVoter("Harshi", 3, 0));

    int choice;
    do {
        printf("\n--- Voting System ---\n");
        printf("1. Start Voting\n");
        printf("2. Display Results\n");
        printf("3. Admin Menu\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                startVoting(&votingQueue);
                break;
            case 2:
                displayResults();
                break;
            case 3:
                adminMenu();
                break;
            case 4:
                printf("Exiting Voting System.\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (choice != 4);

    return 0;
}