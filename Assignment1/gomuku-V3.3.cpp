﻿#define _CRT_SECURE_NO_WARNINGS
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
//6层5子优化后版

//VERSION1.0:初步实现alphabeta剪枝叶算法，采用先前作业使用过的unordered_set来优化遍历棋盘，原理是把坐标用哈希的形式存储，降低复杂度，同时鉴于五子棋的性质，把检索范围聚焦在有棋位置的
//附近4格（本来想尝试2格，但效果很差）其实用4格已经接近完全遍历了。。但是用哈希存储总会快点。后续优化方向：1.优化存储的方法，应该存在比现有方法更好的遍历方法；2.优化评分标准，注意到
//在评分时棋型分数对最终结果的影响很大，初步想法是针对不同的情况对不同棋型进行动态的赋分，例如先拿黑棋优先攻击？有待继续优化。3.针对棋盘边缘的算法有待优化，提前排除掉完全不可能的值
//将其从表中抹去。
//
//VERSION1.1：1.观察输的局棋盘，发现自己的算法在面对地方活三时不选择防守而是追求进攻，因此添加防守系数2，在敌方活3时增加3连的倍数；2.同时在面对形如x_xx的棋型时，会被视为活2错误赋分。
// 
//VERSION1.2；1.优化当前评分逻辑，由于棋盘为12*12，在board中采用二维数组LineInfo来存储12行，12列，16个正对角线，16个反对角线上黑白子的格式，评分时只遍历当前连珠个数大于2的行/列/对角线。
//2.观察到在棋盘边缘时，会去尝试靠近边缘的位置，即使该位置已经不能形成5连，目前考虑评分时针对边缘情况进行优化，例如靠近边缘时进行减分；（待办）
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//VERSION 2.0:1.在进行1.2版本优化时由于当前计分函数过于繁琐，重新完成棋型评估部分。同时，先前的代码的棋型分数判断过于简单，现由于遍历方法改为按条遍历，对于大于2的条进行棋型丰富，用1代表空位
//2代表黑3代表白9代表棋盘边缘(空格不用0是避免前导0影响棋型判断），将棋型用unordered_map的方式和对应的分数提前存储，在评分时直接与棋型表进行对照来给分，在定义评分时，为了避免出现如活三不堵的
// 必胜情况，提前根据棋型进行赋分，完成“杀棋”策略。
//更新：本来是打算用直接哟unorderedmap来存储尽可能多的棋型并进行赋分的，但是发现如果固定棋型的长度，则在遍历时会出现很多遗漏的情况导致无法赋分，ai说可以考虑使用默认的评估函数，但这又回到了原
// 来的版本了，目前我的想法是，评估函数仍然按照原先的按条遍历通过连续值和封堵情况进行赋分，而原本打算存储棋型的哈希表用来存储威胁情况，例如两边为空的活三等等。在接受到place命令后立马进行威胁
// 判断，如果存在威胁情况，不适用alphabeta剪枝算法，直接进行杀棋，避免出现对方必胜的局面。
//更新：发现其实只需要判断三个以上的情况即可
// 结果效果更差了。。。。
// 
// 本以为棋型表会优化算法，但是更改成这种方法后分数成了决定性的因素，虽然有时候会下出出乎意料的好棋但总体效果差。
//一些想法：是否可以对整个棋盘的位置进行评分而不是对整个棋盘的棋型进行评分，能否针对整个棋盘的不同位置，给出例如该位置的进攻分数？防守分数？然后每次落子以后动态更新那个棋子附近的分数即可。
//可以在现有这个2维数组的基础上，对某个点位进行评分就是对其所在行列和两条对角线进行评分也不用依赖棋型表，每次落子的更新该位置和周围八个位置的分数即可。然后通过对棋盘的位置进行剪枝，
//
//VERSION 2.1:思考后发现针对棋子评分一个点要考虑八个方向，且每次下子后还要考虑到传递等等因素，继续沿用按线评分的方法，但完全重构了棋型判断，用哈希图对照的方式暂时保留代码，等到下个版本用来匹配威胁棋型，
// 出现威胁棋型直接先处理。（待完善有关代码）
//VERSION 2.2:添加了威胁判断部分代码，每次一旦出现我方必胜棋型杀棋，但杀棋时候的优先级还可以调整，目前仅仅是两个不同优先级别，当对方出现必胜棋时也进行调整，让其直接封堵避免出现必胜棋型。
//VERSION 2.3:为不同的威胁位置添加了权重占比，在返回时优先返回权重高（危险）的位置。
// 待办优化事项：1.优化ab剪枝逻辑；2.可用位置存储的优化，目前剪枝中不更新可用位置导致无用的遍历，先前更新过但会弹窗报错，如果还是报错尝试更换数据结构存储可用位置，同时我觉得只考虑周围2圈是一个合理的方案，
// 尝试改回来。3.评分的优化，把评分直接存储，每次落子时其实只需要重新算四个条的分数即可。4.威胁识别算法的优化，加入双重威胁的识别，同时把所有可用空位都存储到某个点中，根据这些点的威胁指数（暂时在考虑怎么算）
// 选择最好的点。
// VERSION 2.4:由于在深层递归中会出现迭代器错误，更新了储存可用位置的数据结构，优化了alphabeta剪枝逻辑，删除一部分冗余代码。
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// VERSION 3.0
// 重写了alphabeta剪枝算法加入启发式搜索，优先遍历评分较高的位置，完全重构了评分方法，在详细测量后似乎还是采用精确评分得到的准确率更高。每次落子在总分上重新刷新4个方向的分数更新在总分中，总分作为变量直接存储，
// 省去了每次遍历的过程，优化了性能，alphabeta的性能目前递归能达到7层，采用Zorbist键值表优化存储，把威胁检测加回来了，本质还是因为当前评分算法的劣势。。启发式评分加入了防御评分，效果大大增强。先尝试一次7的深度会不会超时。
//
// VERSION 3.1 修复了若干bug，本来想为当前程序加入杀棋算法，但是算法效果较差.
// 
// VERSION 3.2 再次优化了评分函数，减少计算量，目前深度达到8层4子，终于抛弃了威胁函数，目前算法能正确处理威胁情况， 应该不需要威胁函数的辅助了。原本计划用于杀棋算法的函数暂时用于启发式评估，加重杀棋位置的权重
// 但该版本多次实验后发现还是会出现忽略威胁的行为，在多次debug调试后发现问题出现在两个敌方1.错误的下标，其实就是把y写成x了没发现；2.针对长度小于4的对角线的特殊处理，当前没有进行特殊判断；3.加重杀棋权重时由于采用的是乘法
// 而当前评分表对于堵了一边的三子其实是不进行计分的，而原本的在启发式评估时进行模拟落子在现在由于更改计分逻辑后会增加很多无用的计算量，就会导致某位值在启发式评估分数数值较小的情况下会出现即使乘5依然分数较小。
// 
// VERSION 3.3 修复了部分评分函数的bug，之前的评分函数在计算列时会在错误的下标位置计分，在计算对角线时会把长度小于5的对角线重复计算到不属于他的下标里，所以会出现数组越界的情况（大概率是右下部分位置的反对角线）
// 同时在启发式评估时加大了杀棋位置的比重，比先前的效果好了很多，大概率是终于能抛弃威胁识别函数了，目前仅依靠启发式搜索+杀棋判断来进行alphabeta剪枝，效果似乎还不错
// 更新：修复了122222这样的5连在棋形识别时被错误识别为冲4的问题，为键值表添加深度判断逻辑
// 更新：修复了棋盘边缘棋形无法被正确判断的问题。
// 更新：改成8子后效果也就一般，少量修改评分函数降到6层试试
// 
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

const int SCORE_FIVE = 1000000;
const int SCORE_FOUR = 100000;//两头无堵的4连
const int SCORE_BLOCKED_FOUR = 50000;//一端无堵的4连

const int SCORE_THREE = 10000;//两头无堵的3连
const int SCORE_BLOCKED_THREE = 1000;//堵了一边的3连
const int SCORE_TWO = 100;//两头无堵的2连
const int SCORE_ONE = 10;

const double TIME_LIMIT = 1.5;
const int MAX_DEPTH = 6;

const int HASH_EXACT = 0;
const int HASH_ALPHA = 1;
const int HASH_BETA = 2;
const int UNKNOWN = -999999;

int myFlag;
int enemyFlag;

const int BOARD_MIDDLE_1 = 5;
const int BOARD_MIDDLE_2 = 6;

struct HashEntry {
    int score;        // 棋盘的评分
    int flag;         // 哈希类型标志
    int depth;
};

std::vector<std::vector<std::vector<uint64_t>>> zobristTable(BOARD_SIZE, std::vector<std::vector<uint64_t>>(BOARD_SIZE, std::vector<uint64_t>(3, 0)));
std::unordered_map<uint64_t, HashEntry> transpositionTable;

uint64_t currentHash = 0;

const std::unordered_map<int, int> ScoreMap = {
    // 16种评分
    {22222, SCORE_FIVE},// XXXXX
    {122222,SCORE_FIVE},
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

// 初始化 Zobrist 哈希表
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

void storeHashEntry(uint64_t hashValue, int score, int flag, int depth) {
    transpositionTable[hashValue] = { score,flag,depth };
}

int retrieveHashEntry(uint64_t hashValue, int& alpha, int& beta, int depth) {
    if (transpositionTable.find(hashValue) != transpositionTable.end()) {
        HashEntry entry = transpositionTable[hashValue];
        if (entry.depth >= depth)
        {
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
    }
    return UNKNOWN;
}

struct Coordinate {
    int x;
    int y;
    int score;
    //默认构造函数
    Coordinate() :x(-1), y(-1), score(0) {}
    // 带参构造函数
    Coordinate(int row, int col) :x(row), y(col), score(0) {}
    Coordinate(int row, int col, int _score) :x(row), y(col), score(_score) {}
    bool operator==(const Coordinate& other) const {
        return x == other.x && y == other.y;
    }
    bool operator<(const Coordinate& other) const {
        if (score != other.score) {
            return score > other.score; // 按分数降序排序
        }
        else if (x != other.x) {
            return x < other.x;  // 若分数相同，则按 x 坐标升序排序
        }
        else {
            return y < other.y;  // 若分数和 x 坐标都相同，则按 y 坐标升序排序
        }
    }
};
// 自定义哈希函数对象
struct CoordinateHash {
    std::size_t operator()(const Coordinate& coord) const {
        // 使用组合哈希方法
        std::size_t hx = std::hash<int>()(coord.x);
        std::size_t hy = std::hash<int>()(coord.y);
        return hx ^ (hy << 1); // 位移以避免哈希冲突
    }
};

class Board {
public:
    int board[BOARD_SIZE][BOARD_SIZE] = { {0} };//棋盘布局
    //int line_info[2][54] = { {0} };//记录黑子白子在行列对角线的个数，对角线只考虑长度大于等于5的
    bool _checkpos[BOARD_SIZE][BOARD_SIZE] = { {0} };
    int currentBoardScore[2] = { 0 };//记录当前棋盘对双方的分数，初始存为120（活2分数）
    int currentlineScores[2][54] = { {0} };//记录当前双方每一横数双对角线分数

    int rowIndex(int row) { return row; }
    int colIndex(int col) { return 12 + col; }
    int diagIndex(int row, int col) { return 31 - (row - col); }
    int antiDiagIndex(int row, int col) { return 39 + (row + col - 4); }
    void UpdateCheckPos(int x, int y);
    void UndoUpdateCheckPos(int x, int y);
    void UpdateBoardScore(int x, int yint, int player);//每次落子时更新棋盘分数
    void undoUpdateScore(int x, int y);
    void UpdateLinescores(int x, int y, int player);
    Board() {
        initializeZobristTable();
        MakeMove(BOARD_MIDDLE_1, BOARD_MIDDLE_2, BLACK);
        MakeMove(BOARD_MIDDLE_2, BOARD_MIDDLE_1, BLACK);
        MakeMove(BOARD_MIDDLE_1, BOARD_MIDDLE_1, WHITE);
        MakeMove(BOARD_MIDDLE_2, BOARD_MIDDLE_2, WHITE);

    }
    void MakeMove(int x, int y, int player)
    {
        UpdateBoardScore(x, y, player);
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
int detectKillMove(int player, Board& A, Coordinate& CurrentMove);

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
            if (newX >= 0 && newX < BOARD_SIZE && newY >= 0 && newY < BOARD_SIZE && board[newX][newY] == EMPTY) {
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

void Board::UpdateBoardScore(int x, int y, int player) {
    //减去原本四列的值
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

void Board::UpdateLinescores(int x, int y, int player) {
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
//用于给某个位置的—|\/进行评分，更新棋盘总分，每次落子时更新棋盘分数
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
static bool judgeInRange(Coordinate temp) { //判断位置是否超出棋盘
    if (temp.x < 0)return false;
    if (temp.y < 0)return false;
    if (temp.x > BOARD_SIZE)return false;
    if (temp.y > BOARD_SIZE)return false;
    return true;
}

void GetDir(Coordinate& temp, int& index, int& dRow, int& dCol) {
    if (index < 12) { // 行
        temp.x = index;
        temp.y = 0;
        dRow = 0;
        dCol = 1;
    }
    else if (index < 24) { // 列
        temp.x = 0;
        temp.y = index - 12;
        dRow = 1;
        dCol = 0;
    }
    else if (index < 39) { // 正对角线
        int offset = index - 24 - 7;
        temp.x = std::max(0, -offset);
        temp.y = std::max(0, offset);
        dRow = 1;
        dCol = 1;
    }
    else { // 反对角线
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

    // 根据 index 的范围确定遍历方向
    Coordinate temp;
    int dRow, dCol;
    GetDir(temp, index, dRow, dCol);
    std::vector<int> window(6, 1);  // 长度为6的窗口

    window[0] = 9;

    // 初始化前5和6个位置的窗口
    for (int i = 1; i < 6 && judgeInRange(temp); ++i) {
        window[i] = (A.board[temp.x][temp.y] == EMPTY) ? 1 :
            (A.board[temp.x][temp.y] == player) ? 2 : 3;
        temp.x += dRow;
        temp.y += dCol;
    }
    Coordinate temp5(temp.x - 1, temp.y - 1);//
    // 滑动窗口，逐一移动位置，直到 window6[5] == 9
    while (window[4] != 9) {
        int pattern5 = 0, pattern6 = 0;
        bool matched = false;

        // 将窗口内容转换为整数编码
        for (int i = 0; i < 5; ++i) {
            pattern5 = pattern5 * 10 + window[i];
        }
        pattern6 = pattern5 * 10 + window[5];

        // 检查当前5和6的窗口是否存在于ScoreMap中
        if (ScoreMap.find(pattern5) != ScoreMap.end()) {
            lineScore += ScoreMap.at(pattern5);
            matched = true;
        }
        if (ScoreMap.find(pattern6) != ScoreMap.end()) {
            lineScore += ScoreMap.at(pattern6);
            matched = true;
        }

        // 如果匹配成功，分别跳过相应的窗口长度继续检查
        if (matched) {
            for (int i = 0; i < 5; ++i) {
                if (i == 0 && window[5] == 1) {
                    window[0] = window[5];//当前匹配模式成功且窗口最后一项为empty
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

        // 移动窗口：移除旧的，添加新的
        for (int i = 0; i < 5; ++i) {
            window[i] = window[i + 1];
        }

        if (judgeInRange(temp)) {
            window[5] = (A.board[temp.x][temp.y] == EMPTY) ? 1 :
                (A.board[temp.x][temp.y] == player) ? 2 : 3;
        }
        else {
            window[5] = 9; // 棋盘外
        }
        // 移动到下一个位置
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
    // 使用最小堆来保存前10个最高分的坐标
    auto cmp = [](const Coordinate& a, const Coordinate& b) {
        return a.score > b.score;
        };
    std::priority_queue<Coordinate, std::vector<Coordinate>, decltype(cmp)> topMoves(cmp);

    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            if (A._checkpos[row][col] && A.board[row][col] == EMPTY) {
                int moveScore = evaluateMove(A, row, col, player, true);
                if (!killermode) {
                    moveScore += evaluateMove(A, row, col, opponent, true);//该位置防守评分
                }
                Coordinate move(row, col, moveScore);
                if (moveScore > SCORE_BLOCKED_THREE)
                {
                    move.score += detectKillMove(player, A, move);
                    move.score += detectKillMove(opponent, A, move);
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

    // 将堆中的元素转存为向量并按降序排序
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

int detectKillMove(int player, Board& A, Coordinate& CurrentMove) {
    const std::unordered_set<int> killPatterns = { 122221, 122121,122222, 121221,12222,22221,22122,21222,22212,22222 }; // 常见杀棋棋型
    int fourcount = 0;
    int fivecount = 0;
    int threecount = 0;
    A.board[CurrentMove.x][CurrentMove.y] = player;
    Coordinate temp;
    int index[4] = { A.rowIndex(CurrentMove.x), A.colIndex(CurrentMove.y), A.diagIndex(CurrentMove.x, CurrentMove.y), A.antiDiagIndex(CurrentMove.x, CurrentMove.y) };
    int dRow, dCol;

    for (int i = 0; i < 4; i++) {
        if (i == 2 && (CurrentMove.x - CurrentMove.y < -7 || CurrentMove.x - CurrentMove.y>7)) {
            continue;
        }
        if (i == 3 && (CurrentMove.x + CurrentMove.y < 4 || CurrentMove.x + CurrentMove.y>18)) {
            continue;
        }
        GetDir(temp, index[i], dRow, dCol);
        std::vector<int> window(6, 0);  // 长度为6的窗口
        Coordinate start(CurrentMove.x - 5 * dRow, CurrentMove.y - 5 * dCol);//从当前位置往前四个位置开始遍历，避免判断到该条上其他位置的杀棋
        if (judgeInRange(start)) {
            temp = start;
        }
        Coordinate end(CurrentMove.x + 7 * dRow, CurrentMove.y + 7 * dCol);
        // 初始化窗口的前6个位置
        for (int k = 0; k < 6 && judgeInRange(temp); ++k) {
            window[k] = (A.board[temp.x][temp.y] == EMPTY) ? 1 : (A.board[temp.x][temp.y] == player) ? 2 : 3;
            temp.x += dRow;
            temp.y += dCol;
        }

        // 滑动窗口遍历整个行/列/对角线
        while (window[4] != 9) {
            bool matched = false;
            bool to_end = false;
            if (temp == end) {//遍历到模拟落子位置的后4个位置
                break;
            }
            int pattern5 = 0, pattern6 = 0;

            // 前5位转换为一个数
            for (int k = 0; k < 5; ++k) {
                pattern5 = pattern5 * 10 + window[k];
            }
            pattern6 = pattern5 * 10 + window[5];  // 第6位单独转换

            // 检查当前5位或6位的模式是否为杀棋
            if (killPatterns.count(pattern5) > 0 || killPatterns.count(pattern6) > 0) {
                if (pattern5 == 22222 || pattern6 == 122222)
                {
                    fivecount++;
                }
                else if (pattern6 == 122221 || pattern6 == 122121 || pattern6 == 121221 || pattern5 == 22221 || pattern5 == 22122 || pattern5 == 21222 || pattern5 == 22212)
                {
                    fourcount++;
                }
                else
                {
                    threecount++;
                }
                matched = true;
            }

            // 如果匹配成功，跳过相应的窗口长度继续检查
            if (matched) {
                for (int i = 0; i < 5; ++i) {
                    if (i == 0 && window[5] == 1) {
                        window[0] = window[5];//当前匹配模式成功且窗口最后一项为empty
                        continue;
                    }
                    else {
                        window[i] = (A.board[temp.x][temp.y] == EMPTY) ? 1 :
                            (A.board[temp.x][temp.y] == player) ? 2 : 3;
                        temp.x += dRow;
                        temp.y += dCol;
                        if (temp == end) {//遍历到模拟落子位置的后4个位置
                            to_end = true;
                            break;
                        }
                    }
                }
                if (to_end) {
                    break;
                }
                continue;
            }
            // 移动窗口：移除旧的，添加新的
            for (int k = 0; k < 5; ++k) window[k] = window[k + 1];
            window[5] = (judgeInRange(temp)) ? ((A.board[temp.x][temp.y] == EMPTY) ? 1 : (A.board[temp.x][temp.y] == player) ? 2 : 3) : 9;

            temp.x += dRow;
            temp.y += dCol;
        }
    }
    A.board[CurrentMove.x][CurrentMove.y] = EMPTY;
    int killScore = fourcount * SCORE_FOUR + threecount * SCORE_THREE + fivecount * SCORE_FIVE;
    return killScore;
}

int AlphaBeta(Board& A, int depth, int alpha, int beta, int player, Coordinate& bestMove) {
    uint64_t hashValue = currentHash; // 假设 currentHash 是当前棋盘状态的哈希值
    int score1 = evaluateBoard(A, myFlag);
    int score2 = evaluateBoard(A, enemyFlag);

    if (score1 >= SCORE_FIVE) {
        return MAX_SCORE - 1;
    }
    if (score2 >= SCORE_FIVE) {
        return MIN_SCORE + 1;
    }
    if (depth != MAX_DEPTH) {
        int cachedScore = retrieveHashEntry(hashValue, alpha, beta, MAX_DEPTH - depth);
        if (cachedScore != UNKNOWN) {
            return cachedScore;
        }
    }
    //任意一方取胜直接返回（相当于到达叶节点）
    if (depth == 0) {
        int finalScore;
        finalScore = score1 - score2;
        storeHashEntry(hashValue, finalScore, HASH_EXACT, MAX_DEPTH);
        return finalScore;  // 到达递归深度时返回评估值
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
            if (alpha >= beta) {  // Beta 剪枝
                flag = HASH_BETA;
                break;
            }
            count++;
            if (depth == MAX_DEPTH) {
                if (count >= 6) {
                    break;
                }
            }
            else
            {
                if (count >= 5) {
                    break;
                }
            }
        }
        storeHashEntry(hashValue, value, flag, MAX_DEPTH - depth);
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
            if (beta <= alpha) {  // Alpha 剪枝
                flag = HASH_ALPHA;
                break;
            }
            count++;
            if (count >= 5) {
                break;
            }
        }
        storeHashEntry(hashValue, value, flag, MAX_DEPTH - depth);
        return value;
    }
}

Coordinate getBestmove(Board& A, Coordinate command) {
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
    return 0;
}