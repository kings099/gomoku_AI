#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <array>
#include <vector>
#include <utility>
#include <algorithm>
#include <iostream>
#include <string>
#include <random>
#include <time.h>
//7��6���Ż����

//VERSION1.0:����ʵ��alphabeta��֦Ҷ�㷨��������ǰ��ҵʹ�ù���unordered_set���Ż��������̣�ԭ���ǰ������ù�ϣ����ʽ�洢�����͸��Ӷȣ�ͬʱ��������������ʣ��Ѽ�����Χ�۽�������λ�õ�
//����4�񣨱����볢��2�񣬵�Ч���ܲ��ʵ��4���Ѿ��ӽ���ȫ�����ˡ��������ù�ϣ�洢�ܻ��㡣�����Ż�����1.�Ż��洢�ķ�����Ӧ�ô��ڱ����з������õı���������2.�Ż����ֱ�׼��ע�⵽
//������ʱ���ͷ��������ս����Ӱ��ܴ󣬳����뷨����Բ�ͬ������Բ�ͬ���ͽ��ж�̬�ĸ��֣��������ú������ȹ������д������Ż���3.������̱�Ե���㷨�д��Ż�����ǰ�ų�����ȫ�����ܵ�ֵ
//����ӱ���Ĩȥ��
//
//VERSION1.1��1.�۲���ľ����̣������Լ����㷨����Եط�����ʱ��ѡ����ض���׷������������ӷ���ϵ��2���ڵз���3ʱ����3���ı�����2.ͬʱ���������x_xx������ʱ���ᱻ��Ϊ��2���󸳷֡�
// 
//VERSION1.2��1.�Ż���ǰ�����߼�����������Ϊ12*12����board�в��ö�ά����LineInfo���洢12�У�12�У�16�����Խ��ߣ�16�����Խ����Ϻڰ��ӵĸ�ʽ������ʱֻ������ǰ�����������2����/��/�Խ��ߡ�
//2.�۲쵽�����̱�Եʱ����ȥ���Կ�����Ե��λ�ã���ʹ��λ���Ѿ������γ�5����Ŀǰ��������ʱ��Ա�Ե��������Ż������翿����Եʱ���м��֣������죩
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//VERSION 2.0:1.�ڽ���1.2�汾�Ż�ʱ���ڵ�ǰ�Ʒֺ������ڷ�����������������������֡�ͬʱ����ǰ�Ĵ�������ͷ����жϹ��ڼ򵥣������ڱ���������Ϊ�������������ڴ���2�����������ͷḻ����1�����λ
//2�����3�����9�������̱�Ե(�ո���0�Ǳ���ǰ��0Ӱ�������жϣ�����������unordered_map�ķ�ʽ�Ͷ�Ӧ�ķ�����ǰ�洢��������ʱֱ�������ͱ���ж��������֣��ڶ�������ʱ��Ϊ�˱��������������µ�
// ��ʤ�������ǰ�������ͽ��и��֣���ɡ�ɱ�塱���ԡ�
//���£������Ǵ�����ֱ��Ӵunorderedmap���洢�����ܶ�����Ͳ����и��ֵģ����Ƿ�������̶����͵ĳ��ȣ����ڱ���ʱ����ֺܶ���©����������޷����֣�ai˵���Կ���ʹ��Ĭ�ϵ����������������ֻص���ԭ
// ���İ汾�ˣ�Ŀǰ�ҵ��뷨�ǣ�����������Ȼ����ԭ�ȵİ�������ͨ������ֵ�ͷ��������и��֣���ԭ������洢���͵Ĺ�ϣ�������洢��в�������������Ϊ�յĻ����ȵȡ��ڽ��ܵ�place��������������в
// �жϣ����������в�����������alphabeta��֦�㷨��ֱ�ӽ���ɱ�壬������ֶԷ���ʤ�ľ��档
//���£�������ʵֻ��Ҫ�ж��������ϵ��������
// ���Ч�������ˡ�������
// 
// ����Ϊ���ͱ���Ż��㷨�����Ǹ��ĳ����ַ�����������˾����Ե����أ���Ȼ��ʱ����³��������ϵĺ��嵫����Ч���
//һЩ�뷨���Ƿ���Զ��������̵�λ�ý������ֶ����Ƕ��������̵����ͽ������֣��ܷ�����������̵Ĳ�ͬλ�ã����������λ�õĽ������������ط�����Ȼ��ÿ�������Ժ�̬�����Ǹ����Ӹ����ķ������ɡ�
//�������������2ά����Ļ����ϣ���ĳ����λ�������־��Ƕ����������к������Խ��߽�������Ҳ�����������ͱ�ÿ�����ӵĸ��¸�λ�ú���Χ�˸�λ�õķ������ɡ�Ȼ��ͨ�������̵�λ�ý��м�֦��
//
//VERSION 2.1:˼�����������������һ����Ҫ���ǰ˸�������ÿ�����Ӻ�Ҫ���ǵ����ݵȵ����أ��������ð������ֵķ���������ȫ�ع��������жϣ��ù�ϣͼ���յķ�ʽ��ʱ�������룬�ȵ��¸��汾����ƥ����в���ͣ�
// ������в����ֱ���ȴ������������йش��룩
//VERSION 2.2:�������в�жϲ��ִ��룬ÿ��һ�������ҷ���ʤ����ɱ�壬��ɱ��ʱ������ȼ������Ե�����Ŀǰ������������ͬ���ȼ��𣬵��Է����ֱ�ʤ��ʱҲ���е���������ֱ�ӷ�±�����ֱ�ʤ���͡�
//VERSION 2.3:Ϊ��ͬ����вλ�������Ȩ��ռ�ȣ��ڷ���ʱ���ȷ���Ȩ�ظߣ�Σ�գ���λ�á�
// �����Ż����1.�Ż�ab��֦�߼���2.����λ�ô洢���Ż���Ŀǰ��֦�в����¿���λ�õ������õı�������ǰ���¹����ᵯ������������Ǳ����Ը������ݽṹ�洢����λ�ã�ͬʱ�Ҿ���ֻ������Χ2Ȧ��һ������ķ�����
// ���ԸĻ�����3.���ֵ��Ż���������ֱ�Ӵ洢��ÿ������ʱ��ʵֻ��Ҫ�������ĸ����ķ������ɡ�4.��вʶ���㷨���Ż�������˫����в��ʶ��ͬʱ�����п��ÿ�λ���洢��ĳ�����У�������Щ�����вָ������ʱ�ڿ�����ô�㣩
// ѡ����õĵ㡣
// VERSION 2.4:���������ݹ��л���ֵ��������󣬸����˴������λ�õ����ݽṹ���Ż���alphabeta��֦�߼���ɾ��һ����������롣
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// VERSION 3.0
// ��д��alphabeta��֦�㷨��������ʽ���������ȱ������ֽϸߵ�λ�ã���ȫ�ع������ַ���������ϸ�������ƺ����ǲ��þ�ȷ���ֵõ���׼ȷ�ʸ��ߡ�ÿ���������ܷ�������ˢ��4������ķ����������ܷ��У��ܷ���Ϊ����ֱ�Ӵ洢��
// ʡȥ��ÿ�α����Ĺ��̣��Ż������ܣ�alphabeta������Ŀǰ�ݹ��ܴﵽ7�㣬����Zorbist��ֵ���Ż��洢������в���ӻ����ˣ����ʻ�����Ϊ��ǰ�����㷨�����ơ�������ʽ���ּ����˷������֣�Ч�������ǿ���ȳ���һ��7����Ȼ᲻�ᳬʱ��
//
// VERSION 3.1 �޸�������bug��������Ϊ��ǰ�������ɱ���㷨�������㷨Ч���ϲ�.
// 
// VERSION 3.2 �ٴ��Ż������ֺ��������ټ�������Ŀǰ��ȴﵽ8��4�ӣ�������������в������Ŀǰ�㷨����ȷ������в����� Ӧ�ò���Ҫ��в�����ĸ����ˡ�ԭ���ƻ�����ɱ���㷨�ĺ�����ʱ��������ʽ����������ɱ��λ�õ�Ȩ��
// ���ð汾���ʵ����ֻ��ǻ���ֺ�����в����Ϊ���ڶ��debug���Ժ�����������������з�1.������±꣬��ʵ���ǰ�yд��x��û���֣�2.��Գ���С��4�ĶԽ��ߵ����⴦����ǰû�н��������жϣ�3.����ɱ��Ȩ��ʱ���ڲ��õ��ǳ˷�
// ����ǰ���ֱ���ڶ���һ�ߵ�������ʵ�ǲ����мƷֵģ���ԭ����������ʽ����ʱ����ģ���������������ڸ��ļƷ��߼�������Ӻܶ����õļ��������ͻᵼ��ĳλֵ������ʽ����������ֵ��С������»���ּ�ʹ��5��Ȼ������С��
// 
// VERSION 3.3 �޸��˲������ֺ�����bug��֮ǰ�����ֺ����ڼ�����ʱ���ڴ�����±�λ�üƷ֣��ڼ���Խ���ʱ��ѳ���С��5�ĶԽ����ظ����㵽�����������±�����Ի��������Խ������������������²���λ�õķ��Խ��ߣ�
// ͬʱ������ʽ����ʱ�Ӵ���ɱ��λ�õı��أ�����ǰ��Ч�����˺ܶ࣬�������������������вʶ�����ˣ�Ŀǰ����������ʽ����+ɱ���ж�������alphabeta��֦��Ч���ƺ�������
//

// board information
#define BOARD_SIZE 12
#define EMPTY 0
#define BLACK 1
#define WHITE 2
#define INF 2147483647
#define _INF -2147483647
#define MAX_SCORE 10000000
#define MIN_SCORE -10000000

// command information
#define START "START"
#define PLACE "PLACE"
#define TURN "TURN"
#define END "END"

const int KILL_POS = 1000;//ɱ��λ��
const int PRIORITY_THREAT = 100; // �����в����
const int HIGH_THREAT = 10; // ����в����
const int NOMAL_THREAT = 1; //һ����в


const int SCORE_FIVE = 500000;
const int SCORE_FOUR = 100000;//��ͷ�޶µ�4��
const int SCORE_BLOCKED_FOUR = 80000;//һ���޶µ�4��

const int SCORE_THREE = 50000;//��ͷ�޶µ�3��
const int SCORE_BLOCKED_THREE = 1000;//����һ�ߵ�3��
const int SCORE_TWO = 100;//��ͷ�޶µ�2��
const int SCORE_ONE = 10;

const double TIME_LIMIT = 1.5;
const int MAX_DEPTH = 7;

const int HASH_EXACT = 0;
const int HASH_ALPHA = 1;
const int HASH_BETA = 2;
const int UNKNOWN = -999999;

int myFlag;
int enemyFlag;

const int BOARD_MIDDLE_1 = 5;
const int BOARD_MIDDLE_2 = 6;

struct HashEntry {
    int score;        // ���̵�����
    int flag;         // ��ϣ���ͱ�־
};

std::vector<std::vector<std::vector<uint64_t>>> zobristTable(BOARD_SIZE, std::vector<std::vector<uint64_t>>(BOARD_SIZE, std::vector<uint64_t>(3, 0)));
std::unordered_map<uint64_t, HashEntry> transpositionTable;

uint64_t currentHash = 0;

const std::unordered_map<int, int> ScoreMap = {
    // 16������
    {22222, SCORE_FIVE},// XXXXX
    {122221,SCORE_FOUR},// _XXXX_
    {122211, SCORE_THREE},// _XXX__
    {112221, SCORE_THREE},// __XXX_
    {122121,SCORE_THREE},//  _XX_X_
    {121221, SCORE_THREE},// _X_XX_
    {22221, SCORE_BLOCKED_FOUR},//XXXX_
    {12222,SCORE_BLOCKED_FOUR},//_XXXX
    {22122,SCORE_BLOCKED_FOUR},//XX_XX
    {21222,SCORE_BLOCKED_FOUR},//X_XXX
    {22212,SCORE_BLOCKED_FOUR},//XXX_X
    {112211,SCORE_TWO},//__XX__
    {112121,SCORE_TWO},//__X_X_
    {121211,SCORE_TWO},//_X_X__
    {111211,SCORE_ONE},//___X__
    {112111,SCORE_ONE},//__X___
};

// ��ʼ�� Zobrist ��ϣ��
void initializeZobristTable() {
    std::mt19937_64 rng(std::random_device{}());
    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            zobristTable[row][col][EMPTY] = rng();
            zobristTable[row][col][BLACK] = rng();
            zobristTable[row][col][WHITE] = rng();
        }
    }
}

void updateHash(int row, int col, int piece) {
    currentHash ^= zobristTable[row][col][piece];
}

void storeHashEntry(uint64_t hashValue, int score, int flag) {
    transpositionTable[hashValue] = { score,flag };
}

int retrieveHashEntry(uint64_t hashValue, int &alpha, int &beta) {
    if (transpositionTable.find(hashValue) != transpositionTable.end()) {
        HashEntry entry = transpositionTable[hashValue];
        if (entry.flag == HASH_EXACT) {
            return entry.score;
        }
        else if (entry.flag == HASH_ALPHA && entry.score <= alpha) {
            return entry.score;
        }
        else if (entry.flag == HASH_BETA && entry.score >= beta) {
            return entry.score;
        }
    }
    return UNKNOWN;
}

struct Coordinate {
    int x;
    int y;
    int score;
    //Ĭ�Ϲ��캯��
    Coordinate() :x(-1), y(-1), score(0) {}
    // ���ι��캯��
    Coordinate(int row, int col) :x(row), y(col), score(0) {}
    Coordinate(int row, int col, int _score) :x(row), y(col), score(_score) {}
    bool operator==(const Coordinate& other) const {
        return x == other.x && y == other.y;
    }
    bool operator<(const Coordinate& other) const {
        if (score != other.score) {
            return score > other.score; // ��������������
        }
        else if (x != other.x) {
            return x < other.x;  // ��������ͬ���� x ������������
        }
        else {
            return y < other.y;  // �������� x ���궼��ͬ���� y ������������
        }
    }
};
struct ThreatInfo {
    Coordinate position; // ��вλ��
    int weight;          // Ȩ��
};

// �Զ����ϣ��������
struct CoordinateHash {
    std::size_t operator()(const Coordinate& coord) const {
        // ʹ����Ϲ�ϣ����
        std::size_t hx = std::hash<int>()(coord.x);
        std::size_t hy = std::hash<int>()(coord.y);
        return hx ^ (hy << 1); // λ���Ա����ϣ��ͻ
    }
};

class Board {
public:
    int board[BOARD_SIZE][BOARD_SIZE] = { {0} };//���̲���
    //int line_info[2][54] = { {0} };//��¼���Ӱ��������жԽ��ߵĸ������Խ���ֻ���ǳ��ȴ��ڵ���5��
    bool _checkpos[BOARD_SIZE][BOARD_SIZE] = { {0} };
    int currentBoardScore[2] = { 0 };//��¼��ǰ���̶�˫���ķ�������ʼ��Ϊ120����2������
    int currentlineScores[2][54] = { {0} };//��¼��ǰ˫��ÿһ����˫�Խ��߷���

    int rowIndex(int row) { return row; }
    int colIndex(int col) { return 12 + col; }
    int diagIndex(int row, int col) { return 31 - (row - col); }
    int antiDiagIndex(int row, int col) { return 39 + (row + col - 4); }
    void UpdateCheckPos(int x, int y);
    void UndoUpdateCheckPos(int x, int y);
    void UpdateBoardScore(int x, int yint,int player);//ÿ������ʱ�������̷���
    void undoUpdateScore(int x, int y);
    void UpdateLinescores(int x,int y,int player);
    Board() {
        initializeZobristTable();
        MakeMove(BOARD_MIDDLE_1, BOARD_MIDDLE_2, BLACK);
        MakeMove(BOARD_MIDDLE_2, BOARD_MIDDLE_1, BLACK);
        MakeMove(BOARD_MIDDLE_1, BOARD_MIDDLE_1, WHITE);
        MakeMove(BOARD_MIDDLE_2, BOARD_MIDDLE_2, WHITE);

    }
    void MakeMove(int x, int y, int player)
    {
        UpdateBoardScore(x, y,player);
        UpdateCheckPos(x, y);
        updateHash(x, y, player);
    }
    void UndoMove(int x, int y) {
        updateHash(x, y, board[x][y]);
        undoUpdateScore(x, y);
        UndoUpdateCheckPos(x, y);
    }
};
int evaluateMove(Board& A, int x, int y, int currentPlayer, bool is_simulate);
int CalculateLineScore(Board& A, int player, int index);
bool detectKillMove(int player, Board& A, Coordinate& CurrentMove);

void Board::UpdateCheckPos(int x, int y) {
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            int newX = x + i;
            int newY = y + j;
            if (newX >= 0 && newX < BOARD_SIZE && newY >= 0 && newY < BOARD_SIZE && board[newX][newY] == EMPTY) {
                _checkpos[newX][newY] = true;
            }
        }
    }
    _checkpos[x][y] = false;
}

void Board::UndoUpdateCheckPos(int x, int y) {
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            int newX = x + i;
            int newY = y + j;
            if (newX >= 0 && newX < BOARD_SIZE && newY >= 0 && newY < BOARD_SIZE && board[newX][newY]==EMPTY) {
                bool hasAdjacentPiece = false;
                for (int m = -1; m < 2; m++) {
                    for (int n = -1; n < 2; n++) {
                        int adjX = newX + m;
                        int adjY = newY + n;
                        if (adjX >= 0 && adjX < BOARD_SIZE && adjY >= 0 && adjY < BOARD_SIZE && board[adjX][adjY] != EMPTY) {
                            hasAdjacentPiece = true;
                            break;
                        }
                    }
                    if (hasAdjacentPiece) {
                        _checkpos[newX][newY] = true;
                        break;
                    }
                }
                if (!hasAdjacentPiece) {
                    _checkpos[newX][newY] = false;
                }
            }
        }
    }
}

void Board::UpdateBoardScore(int x, int y,int player) {
    //��ȥԭ�����е�ֵ
    for(int i = 0; i < 2;i++)
    {
        currentBoardScore[i] -= currentlineScores[i][rowIndex(x)];
        currentBoardScore[i] -= currentlineScores[i][colIndex(y)];
        if (x - y >= -7 && y - x <= 7) {
            currentBoardScore[i] -= currentlineScores[i][diagIndex(x, y)];
        }
        if (x + y >= 4 && x + y <= 18) {
            currentBoardScore[i] -= currentlineScores[i][antiDiagIndex(x, y)];
        }
    }
    board[x][y] = player;
    UpdateLinescores(x, y, WHITE);
    UpdateLinescores(x, y, BLACK);
    for (int i = 0; i < 2; i++)
    {
        currentBoardScore[i] += currentlineScores[i][rowIndex(x)];
        currentBoardScore[i] += currentlineScores[i][colIndex(y)];
        if (x - y >= -7 && y - x <= 7) {
            currentBoardScore[i] += currentlineScores[i][diagIndex(x, y)];
        }
        if (x + y >= 4 && x + y <= 18) {
            currentBoardScore[i] += currentlineScores[i][antiDiagIndex(x, y)];
        }
    }
}
void Board::undoUpdateScore(int x, int y) {
    for (int i = 0; i < 2; i++)
    {
        currentBoardScore[i] -= currentlineScores[i][rowIndex(x)];
        currentBoardScore[i] -= currentlineScores[i][colIndex(y)];
        if (x - y >= -7 && y - x <= 7) {
            currentBoardScore[i] -= currentlineScores[i][diagIndex(x, y)];
        }
        if (x + y >= 4 && x + y <= 18) {
            currentBoardScore[i] -= currentlineScores[i][antiDiagIndex(x, y)];
        }
    }
    board[x][y] = EMPTY;
    UpdateLinescores(x, y, WHITE);
    UpdateLinescores(x, y, BLACK);
    for (int i = 0; i < 2; i++)
    {
        currentBoardScore[i] += currentlineScores[i][rowIndex(x)];
        currentBoardScore[i] += currentlineScores[i][colIndex(y)];
        if (x - y >= -7 && y - x <= 7) {
            currentBoardScore[i] += currentlineScores[i][diagIndex(x, y)];
        }
        if (x + y >= 4 && x + y <= 18) {
            currentBoardScore[i] += currentlineScores[i][antiDiagIndex(x, y)];
        }
    }

}

void Board::UpdateLinescores(int x, int y,int player) {
    int flag = player == BLACK ? 0 : 1;
    currentlineScores[flag][rowIndex(x)] = CalculateLineScore(*this, player, rowIndex(x));
    currentlineScores[flag][colIndex(y)] = CalculateLineScore(*this, player, colIndex(y));
    if (x - y >= -7 && y - x <= 7) {
        currentlineScores[flag][diagIndex(x, y)] = CalculateLineScore(*this, player, diagIndex(x, y));
    }
    if (x + y >= 4 && x + y <= 18) {
        currentlineScores[flag][antiDiagIndex(x, y)] = CalculateLineScore(*this, player, antiDiagIndex(x, y));
    }
}
//���ڸ�ĳ��λ�õġ�|\/�������֣����������ܷ֣�ÿ������ʱ�������̷���
int evaluateMove(Board& A, int x, int y, int currentPlayer, bool is_simulate) {
    int score = 0;
    int flag = currentPlayer == BLACK ? 0 : 1;
    if (is_simulate) {
        A.MakeMove(x, y, currentPlayer);
    }
    score += A.currentlineScores[flag][A.rowIndex(x)];
    score += A.currentlineScores[flag][A.colIndex(y)];
    if (x - y >= -7 && y - x <= 7) {
        score += A.currentlineScores[flag][A.diagIndex(x, y)];
    }
    if (x + y >= 4 && x + y <= 18) {
        score += A.currentlineScores[flag][A.antiDiagIndex(x, y)];
    }
    if (is_simulate) {
        A.UndoMove(x, y);
    }
    return score;
}
static bool judgeInRange(Coordinate temp) { //�ж�λ���Ƿ񳬳�����
    if (temp.x < 0)return false;
    if (temp.y < 0)return false;
    if (temp.x > BOARD_SIZE)return false;
    if (temp.y > BOARD_SIZE)return false;
    return true;
}

void GetDir(Coordinate& temp, int& index, int& dRow, int& dCol) {
    if (index < 12) { // ��
        temp.x = index;
        temp.y = 0;
        dRow = 0;
        dCol = 1;
    }
    else if (index < 24) { // ��
        temp.x = 0;
        temp.y = index - 12;
        dRow = 1;
        dCol = 0;
    }
    else if (index < 39) { // ���Խ���
        int offset = index - 24 - 7;
        temp.x = std::max(0, -offset);
        temp.y = std::max(0, offset);
        dRow = 1;
        dCol = 1;
    }
    else { // ���Խ���
        int offset = index - 39 + 4;
        if (offset < BOARD_SIZE) {
            temp.x = std::max(0, -offset);
            temp.y = std::max(0, offset);
        }
        else {
            offset -= 11;
            temp.x = std::max(0, offset);
            temp.y = std::max(11, -offset);
        }
        dRow = 1;
        dCol = -1;
    }
}
int CalculateLineScore(Board& A, int player, int index) {
    int lineScore = 0;
    int opponent = (player == BLACK) ? WHITE : BLACK;

    // ���� index �ķ�Χȷ����������
    Coordinate temp;
    int dRow, dCol;
    GetDir(temp, index, dRow, dCol);
    std::vector<int> window(6, 1);  // ����Ϊ6�Ĵ���

    window[0] = 9;

    // ��ʼ��ǰ5��6��λ�õĴ���
    for (int i = 1; i < 6 && judgeInRange(temp); ++i) {
        window[i] = (A.board[temp.x][temp.y] == EMPTY) ? 1 :
            (A.board[temp.x][temp.y] == player) ? 2 : 3;
        temp.x += dRow;
        temp.y += dCol;
    }
    Coordinate temp5(temp.x - 1, temp.y - 1);//
    // �������ڣ���һ�ƶ�λ�ã�ֱ�� window6[5] == 9
    while (window[4] != 9) {
        int pattern5 = 0, pattern6 = 0;
        bool matched = false;

        // ����������ת��Ϊ��������
        for (int i = 0; i < 5; ++i) {
            pattern5 = pattern5 * 10 + window[i];
        }
        pattern6 = pattern5 * 10 + window[5];

        // ��鵱ǰ5��6�Ĵ����Ƿ������ScoreMap��
        if (ScoreMap.find(pattern5) != ScoreMap.end()) {
            lineScore += ScoreMap.at(pattern5);
            matched = true;
        }
        if (ScoreMap.find(pattern6) != ScoreMap.end()) {
            lineScore += ScoreMap.at(pattern6);
            matched = true;
        }

        // ���ƥ��ɹ����ֱ�������Ӧ�Ĵ��ڳ��ȼ������
        if (matched) {
            for (int i = 0; i < 5; ++i) {
                if (i == 0 && window[5] == 1) {
                    window[0] = window[5];//��ǰƥ��ģʽ�ɹ��Ҵ������һ��Ϊempty
                    continue;
                }
                else {
                    window[i] = (A.board[temp.x][temp.y] == EMPTY) ? 1 :
                        (A.board[temp.x][temp.y] == player) ? 2 : 3;
                    temp.x += dRow;
                    temp.y += dCol;
                }
            }
            continue;
        }

        // �ƶ����ڣ��Ƴ��ɵģ�����µ�
        for (int i = 0; i < 5; ++i) {
            window[i] = window[i + 1];
        }

        if (judgeInRange(temp)) {
            window[5] = (A.board[temp.x][temp.y] == EMPTY) ? 1 :
                (A.board[temp.x][temp.y] == player) ? 2 : 3;
        }
        else {
            window[5] = 9; // ������
        }
        // �ƶ�����һ��λ��
        temp.x += dRow;
        temp.y += dCol;
    }

    return lineScore;
}
int evaluateBoard(Board& A, int player) {
    if (player == BLACK) {
        return A.currentBoardScore[0];
    }
    else
    {
        return A.currentBoardScore[1];
    }
}

std::vector<Coordinate> generateAndSortMoves(Board& A, int player, bool killermode) {
    int opponent = (player == BLACK) ? WHITE : BLACK;
    // ʹ����С��������ǰ10����߷ֵ�����
    auto cmp = [](const Coordinate& a, const Coordinate& b) {
        return a.score > b.score;
        };
    std::priority_queue<Coordinate, std::vector<Coordinate>, decltype(cmp)> topMoves(cmp);

    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            if (A._checkpos[row][col]&&A.board[row][col]==EMPTY) {
                int moveScore = evaluateMove(A, row, col, player,false);
                if (!killermode) {
                    moveScore +=  evaluateMove(A, row, col, opponent,false);//��λ�÷�������
                }
                Coordinate move(row, col, moveScore);
                if (detectKillMove(opponent, A, move)) {
                    move.score += SCORE_FIVE;
                }
                if (detectKillMove(player, A, move)) {
                    move.score += SCORE_FIVE; //����ɱ�������Ȩ��
                }
                
                if (topMoves.size() < 10) {
                    topMoves.push(move);
                }
                else if (move.score > topMoves.top().score) {
                    topMoves.pop();
                    topMoves.push(move);
                }
            }
        }
    }

    // �����е�Ԫ��ת��Ϊ����������������
    std::vector<Coordinate> moves;
    while (!topMoves.empty()) {
        moves.push_back(topMoves.top());
        topMoves.pop();
    }
    std::sort(moves.begin(), moves.end(), [](const Coordinate& a, const Coordinate& b) {
        return a.score > b.score;
        });
    return moves;

}

bool detectKillMove(int player, Board& A, Coordinate& CurrentMove) {
    const std::unordered_set<int> killPatterns = { 122221, 122121, 121221,12222,22221,22122,21222,22212,22222 }; // ����ɱ������
    A.board[CurrentMove.x][CurrentMove.y] = player;
    Coordinate temp;
    int index[4] = { A.rowIndex(CurrentMove.x), A.colIndex(CurrentMove.y), A.diagIndex(CurrentMove.x, CurrentMove.y), A.antiDiagIndex(CurrentMove.x, CurrentMove.y) };
    int dRow, dCol;

    for (int i = 0; i < 4; i++) {
        if (i == 2 && (CurrentMove.x - CurrentMove.y < -7 || CurrentMove.x - CurrentMove.y>7)) {
            break;
        }
        if (i == 3 && (CurrentMove.x + CurrentMove.y < 4 || CurrentMove.x + CurrentMove.y>18)) {
            break;
        }
        GetDir(temp, index[i], dRow, dCol);
        std::vector<int> window(6, 0);  // ����Ϊ6�Ĵ���

        // ��ʼ�����ڵ�ǰ6��λ��
        for (int k = 0; k < 6 && judgeInRange(temp); ++k) {
            window[k] = (A.board[temp.x][temp.y] == EMPTY) ? 1 : (A.board[temp.x][temp.y] == player) ? 2 : 3;
            temp.x += dRow;
            temp.y += dCol;
        }

        // �������ڱ���������/��/�Խ���
        while (judgeInRange(temp)) {
            int pattern5 = 0, pattern6 = 0;

            // ǰ5λת��Ϊһ����
            for (int k = 0; k < 5; ++k) {
                pattern5 = pattern5 * 10 + window[k];
            }
            pattern6 = pattern5 * 10 + window[5];  // ��6λ����ת��

            // ��鵱ǰ5λ��6λ��ģʽ�Ƿ�Ϊɱ��
            if (killPatterns.count(pattern5) > 0 || killPatterns.count(pattern6) > 0) {
                A.board[CurrentMove.x][CurrentMove.y] = EMPTY;
                return true;  // �ҵ�ɱ������������
            }

            // �ƶ����ڣ��Ƴ��ɵģ�����µ�
            for (int k = 0; k < 5; ++k) window[k] = window[k + 1];
            window[5] = (judgeInRange(temp)) ? ((A.board[temp.x][temp.y] == EMPTY) ? 1 : (A.board[temp.x][temp.y] == player) ? 2 : 3) : 9;

            temp.x += dRow;
            temp.y += dCol;
        }
    }
    A.board[CurrentMove.x][CurrentMove.y] = EMPTY;
    return false;  // û�ҵ�ɱ����򷵻� false
}

int AlphaBeta(Board& A, int depth, int alpha, int beta, int player, Coordinate& bestMove) {
    uint64_t hashValue = currentHash; // ���� currentHash �ǵ�ǰ����״̬�Ĺ�ϣֵ
    int score1 = evaluateBoard(A, myFlag);
    int score2 = evaluateBoard(A, enemyFlag);

    if (score1 >= SCORE_FIVE) {
        return MAX_SCORE - 1;
    }
    if (score2 >= SCORE_FIVE) {
        return MIN_SCORE + 1;
    }
    if (depth != MAX_DEPTH) {
        int cachedScore = retrieveHashEntry(hashValue, alpha, beta);
        if (cachedScore != UNKNOWN) {
            return cachedScore;
        }
    }
    //����һ��ȡʤֱ�ӷ��أ��൱�ڵ���Ҷ�ڵ㣩
    if (depth == 0) {
        int finalScore;
        finalScore = score1 - score2;
        storeHashEntry(hashValue, finalScore, HASH_EXACT);
        return finalScore;  // ����ݹ����ʱ��������ֵ
    }
    std::vector<Coordinate> possiblePositions = generateAndSortMoves(A, player, false);
    int flag = UNKNOWN;
    if (player == myFlag) {  // Maximizing player (AI)
        int value = _INF;
        int count = 0;
        for (const Coordinate& pos : possiblePositions) {

            A.MakeMove(pos.x, pos.y, player);
            value = std::max(value, AlphaBeta(A, depth - 1, alpha, beta, enemyFlag, bestMove));
            if (value > alpha) {
                alpha = value;
                flag = HASH_EXACT;
                if (depth == MAX_DEPTH)
                {
                    bestMove = pos;
                }
            }
            //if (depth==MAX_DEPTH)
            //{
            //    printf("DEBUG CURRENT MYMMOVE %d,%d,score:%d\n", pos.x, pos.y, value);
            //    fflush(stdout);
            //}
            A.UndoMove(pos.x, pos.y);
            if (alpha >= beta) {  // Beta ��֦
                flag = HASH_BETA;
                break;
            }
            count++;
            if (count >= 6) {
                break;
            }
        }
        storeHashEntry(hashValue, value, flag);
        return value;
    }
    else {  // Minimizing player (Opponent)
        int value = INF;
        int count = 0;
        for (const Coordinate& pos : possiblePositions) {
            A.MakeMove(pos.x, pos.y, player);
            //if (depth)
            //{
            //    printf("DEBUG CURRENT OP MOVE: %d,%d,score:%d\n", pos.x, pos.y, value);
            //    fflush(stdout);
            //}
            value = std::min(value, AlphaBeta(A, depth - 1, alpha, beta, myFlag, bestMove));

            if (value < beta) {
                flag = HASH_EXACT;
                beta = value;
            }
            A.UndoMove(pos.x, pos.y);
            if (beta <= alpha) {  // Alpha ��֦
                flag = HASH_ALPHA;
                break;
            }
            count++;
            if (count >= 6) {
                break;
            }
        }
        storeHashEntry(hashValue, value, flag);
        return value;
    }
}

Coordinate getBestmove(Board& A,Coordinate command) {
    Coordinate bestmove;
    std::set<Coordinate> threatPositions;
    AlphaBeta(A, MAX_DEPTH, _INF, INF, myFlag, bestmove);
    if (!judgeInRange(bestmove) || A.board[bestmove.x][bestmove.y] != EMPTY) {
        std::vector<Coordinate> possiblePositions = generateAndSortMoves(A, myFlag, false);
        bestmove = possiblePositions.front();
    }
    return bestmove;
}
void loop()
{
    Board A;
    char tag[10] = { 0 };
    Coordinate command;
    Coordinate BestMove;
    std::vector<Coordinate> threatPositions;
    while (1) {
        memset(tag, 0, sizeof(tag));
        scanf("%s", tag);
        if (strcmp(tag, START) == 0) {
            scanf("%d", &myFlag);
            enemyFlag = 3 - myFlag;
            printf("OK\n");
            fflush(stdout);
        }
        else if (strcmp(tag, PLACE) == 0) {
            scanf("%d %d", &command.x, &command.y);
            A.MakeMove(command.x, command.y, enemyFlag);
        }
        else if (strcmp(tag, TURN) == 0) {
            BestMove = getBestmove(A, command);
            A.MakeMove(BestMove.x, BestMove.y, myFlag);
            printf("%d %d\n", BestMove.x, BestMove.y);
            fflush(stdout);
        }

        else if (strcmp(tag, END) == 0) {
            exit(0);
        }

    }
}
int main()
{
    loop();
    //Board A;
    //A.MakeMove(4, 4, BLACK);
    //A.MakeMove(7, 4, WHITE);
    //A.MakeMove(4, 7, BLACK);
    //A.UndoMove(4, 7);
    //A.MakeMove(4, 7, BLACK);
    //A.MakeMove(8, 4, WHITE);
    //A.MakeMove(4, 6, BLACK);
    return 0;
}