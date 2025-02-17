#ifndef MODEL_H
#define MODEL_H
#include <iostream>
#include <map>
#include "card.h"
#include "gamestate.h"
#include <QRandomGenerator>
#include <QObject>

class Model : public QObject
{
    Q_OBJECT
public:
    GameState gameState;
    int selectedActionCardIDFromPersonalPile;
    bool gameIsStarted;
    explicit Model(QObject *parent = nullptr);

    ///
    /// \brief Method used for testing model functionality.
    ///
    void modelTestMethod();
    ///
    /// \brief HandleChatMessage: receives a chat message, and sends it to all connected players
    /// \param id: id of the sender
    /// \param message: message sent by sender
    ///
    void HandleChatMessage(long long id, QString message);
    ///
    /// \brief HandleTableauSelection: receives the player's selected tableau card's index as a qstring
    /// and calls the last used action card's callback with the sent index as parameter
    /// \param id: socketDescriptor of the sender
    /// \param message: QString representing the player's chosen tableau card's index
    ///
    void HandleTableauSelection(long long id, QString message);

    ///
    /// \brief HandleActionSelection: Handles when a player selects an action card
    /// \param id socketDescriptor of the sender
    /// \param message Qstring which represents the selected action card
    ///
    void HandleActionSelection(long long id, QString message);

    ///
    /// \brief HandlePlayerName: handles when a player specifies their name.
    /// \param id socketDescriptor
    /// \param message name
    ///
    void HandlePlayerName(long long id, QString message);

    ///
    /// \brief HandleStartGame: handles when a player clicks the start game button in view
    /// \param id socketDescriptor
    ///
    void HandleStartGame(long long id);

    ///
    /// \brief HandleSelectedPlayer: handles when a player selects another player to sabotage
    /// \param id socketDescriptor
    /// \param message selected player
    ///
    void HandleSelectedPlayer(long long id, QString message);

    ///
    /// \brief addNewPlayer: creates a new player within the gameState's players vector
    /// \param id
    void addNewPlayer(long long id);

    ///
    /// \brief endGame: ends the game and alerts all clients
    ///
    void endGame();


    ///
    /// \brief movementCardComplete: callback for when a player selects a person card to move
    /// \param indexInTab
    ///
    void movementCardComplete(int indexInTab);

    //specify integer verions of each color specification to clarify parameters
    int blue = 0;
    int green = 1;
    int purple = 2;
    int red = 3;
    int anyColor= -1;
    int numberOfColors= 4;

    ///
    /// \brief scoreManipulatorPlayed: Handles when an action card that manipulates score
    /// \param specifiedColor: effected color as listed above
    /// \param colorScoreBuff: how much the color effects score
    /// \param misc: extra information to be specified by specific function
    ///
    void scoreManipulatorPlayed(int specifiedColor, int colorScoreBuff, int misc);


    ///
    /// \brief cardTuple: tuple to hold information and function pointers associated with an action card
    ///
    typedef std::tuple<void(Model::*)(int, int, int), QVector<int>, void(Model::*)(int)> cardTuple; //function, array of params, callback (reference lines below)

    ///
    /// function pointer associated with an action card
    ///
    typedef void (Model::* cardFunction)(int, int, int);

    ///
    /// function pointer associated with 2-part(further player input) action cards
    ///
    typedef void (Model::* cardCallBack)(int);

    /// ID of the action card that is currently in play
    int currentAID;

    ///
    /// \brief populationActionMap: used to populate the actionMap, a
    /// collection of card index's and their card tuples
    ///
    void populationActionMap();

    ///
    /// \brief peopleTupleL tuple to hold information and function pointers associated with people cards
    /// //cardTuple = correpsonding info (value, color, special func)
    ///
    typedef std::tuple<int, int, void (Model::*)(int)> peopleTuple;

    ///
    /// \brief peopleMap: map of all people card ids and their tuples
    ///
    std::map <int,peopleTuple> peopleMap;

    ///
    /// \brief actionMap: map of all action card ids and their tuples
    ///
    std::map <int,cardTuple> actionMap;

    ///
    /// \brief peopleNameMap: map of people names for chat
    ///
    std::map<int, QString> peopleNameMap;

    ///
    /// \brief calulateColorSum: calculates the total number of the specified color a player has
    /// and its effect on the score
    /// \param color: color we are looking for
    /// \param manipultorEnabled: whether or not the score manipulator is active
    /// \param playerIndex: specified player
    /// \return
    ///
    int calulateColorSum(int color, bool manipultorEnabled, int playerIndex);

    ///
    /// \brief recalculateScore: recalculates and updates the scores of all players/
    ///
    void recalculateScore();


    ///
    /// \brief methods associated with moving cards
    /// all triple void triple int parameter methods are used in actionCardTuples
    ///
    void frontToBack(int, int, int);
    void shuffleTableauPlayed(int, int, int);
    void shuffleTableauPlayed16(int, int, int);
    void shuffleTableauPlayed17(int, int, int);
    void reverseCardPlayed(int, int, int);
    void newLinePlayed(int, int, int);
    void escapeCardPlayed1stPart(int, int, int);
    void escapeCardPlayed2ndPart(int);
    void addToTableau(int numCardsToShuffle, int, int);
    void addFromTopThree(int, int, int);
    void merylToFront(int, int, int);
    void takeDiscardedAction(int, int, int);
    void crewToFront(int, int, int);
    void moveClosestBlueToFront(int, int, int);
    void choosePlayer(int, int, int);
    void mixAfterTurn(int,int,int);
    void swapHandsComplete(int victimPlayer);
    void discardOneAction(int, int, int);
    void dealNewActionCard(int, int, int);
    void allRemoveAnAction(int, int, int);
    void drawThreeActionNoNoble(int, int, int);
    void endDay(int, int, int);
    void blockPlayer(int, int, int);
    void skipPlayer(int, int, int);

    ///
    /// \brief methods associated with specific cards
    ///
    void afterYou(int);
    void replacePerson(int);
    void discardPerson(int);
    void disqualification(int);

    ///
    /// \brief endOfTurn: performs end of turn proceedure including:
    /// incrementing player turn, checking for end of game, recalculating scores,
    /// restarting rounds, drawing new action card, alerting server to send
    /// serialized gameState
    ///
    void endOfTurn();






signals:
    ///
    /// \brief sendChatToPlayers: alerts the server to send chat messages
    ///
    void sendChatToPlayers(QString);

    ///
    /// \brief sendStateToPlayers: alerts the server to send the state to all players
    ///
    void sendStateToPlayers(QByteArray);

    ///
    /// \brief actionCardSelectedFromPersonalPile: alerts the server of the action card
    /// \param selectedActionCard
    ///
    //void actionCardSelectedFromPersonalPile(Card selectedActionCard);

    ///
    /// \brief updateTableauAfterActionCardSelect:
    /// alerts the server to alert the view to update the tableau
    ///
    void updateTableauAfterActionCardSelect();

    ///
    /// \brief sendStateToPlayer: send the state to a specific player
    /// \param playerIndex: index of player to send to
    ///
    void sendStateToPlayer(QByteArray, int playerIndex);

    ///
    /// \brief displayWinnerAndConfetti
    /// alert the server to alert the view to show confetti
    void displayWinnerAndConfetti();


private:
    ///
    /// Function pointer type for action cards, takes 3 generic integer parameters
    /// which are to be defined later
    ///
    typedef void (*actionCardFuncPtr)(int param1, int param2, int param3);

    ///
    /// \brief map from card ID to function.
    /// functions must be of the type actionCardFuncPtr to match
    /// parameters
    ///

    ///
    /// \brief Adds additional points to the current players score calculator
    /// \param victimPlayerID
    /// \param scoreModification
    /// \param unused paramter
    ///
    void addPointsFromActionCard(int scoreModification, int unused1, int unused2);

    ///
    /// \brief Modifies color multiplier of the current players score calculator
    /// \param index of color that will be modified
    /// \param scoreModification the amount the point values of that color are modified
    /// \param unused parameter
    ///
    void addPointsForColor(int color, int scoreModification, int unused1);

    ///
    /// \brief modifies the specified player's score calculator.
    /// \param index of player to be sabotaged
    /// \param scoreModification the amount the victim's score will be affected by
    /// \param unused parameter
    ///
    void decreaseOtherPlayerPoints(int victimPlayerIndex, int scoreModification, int unused1);

    ///
    /// \brief modifies the position of the specified card within the tableau
    /// this method will not be hard coded into the game because it will need to receive
    /// parameters from the client. I.E. it doesn't have the exact same effect every time
    /// \param indexInTableau index of the selected card within the tableau
    /// \param positionModification number of positions the card is being used
    /// \param unused parameter
    ///
    void movementCardPlayed(int requiredColor, int unused1, int unused2);

    ///
    /// \brief For card 29. makes all negative cards no longer detriment score
    ///
    void neutralizeGrays(int unused,int unused1,int unused2);

    ///
    /// \brief This method will replace the line
    /// \param unused1
    /// \param unused2
    /// \param unused3
    ///
    void newTableau(int unused1, int unused2, int unused3);

    ///
    /// \brief fills the game state with the required card ids
    ///
    void populateGameState();

    ///
    /// \brief Method for making a specific player discard a randumb action card
    /// \param victimPlayerIndex
    ///
    void makeDiscardAction(int victimPlayerIndex);

    ///
    /// \brief creates a random tableau and puts it into the game state
    ///  requires that game has been populated
    ///
    void generateRandomTableau(QVector<int> availablePeople, int size);

    ///
    /// \brief populates all player's hands with random action cards
    /// may need to modify this if we allow for less than 4 player to play
    /// for now this is a good test
    ///
    void generateRandomHands();

    ///
    /// \brief This method adds card information to the model
    /// including any special properties
    ///
    void populatePeopleMap();

    ///
    /// \brief drawActionCard: draws the specified number of action cards
    /// \param numberOfCards
    ///
    void drawActionCard(int numberOfCards);


    ///
    /// \brief adds action card information to the model
    /// this includes the function pointers to any
    /// effects they have
    ///
    void populateActionMap();

    ///
    /// \brief shuffles the cards in the tableau without modifying people card stack
    ///
    void shuffleTableau();
};

#endif // MODEL_H
