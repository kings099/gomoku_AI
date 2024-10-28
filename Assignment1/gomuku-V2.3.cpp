#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <utility>
#include <algorithm>
#include <iostream>
#include <string>
#include <time.h>

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
// VERSION 2.4:由于在深层递归中会出现迭代器错误，更新了储存可用位置的数据结构，优化了alphabeta剪枝逻辑，删除一部分冗余代码
//

// board information
#define BOARD_SIZE 12
#define EMPTY 0
#define BLACK 1
#define WHITE 2
#define INF 2147483647
#define _INF -2147483647

// command information
#define START "START"
#define PLACE "PLACE"
#define TURN "TURN"
#define END "END"


const int KILL_POS = 15000;
const int PRIORITY_THREAT = 10000; // 最高威胁级别
const int HIGH_THREAT = 8000; // 高威胁级别
const int NOMAL_THREAT = 5000; //一般威胁


const int SCORE_FIVE = 100000;
const int SCORE_FOUR = 100000;//两头无堵的4连
const int SCORE_BLOCKED_FOUR = 50000;//一端无堵的4连

const int SCORE_THREE = 10000;//两头无堵的3连
const int SCORE_BLOCKED_THREE = 5000;//堵了一边的3连
const int SCORE_TWO = 500;//两头无堵的2连
const int BLOCK_TWO = 100;

const double TIME_LIMIT = 2;
const int MAX_DEPTH = 5;

int myFlag;
int enemyFlag;

const int BOARD_MIDDLE_1 = 5;
const int BOARD_MIDDLE_2 = 6;

const int DEFENSE_MULTIPLIER = 10; // 防守优先级加权


const std::unordered_map<int, int> ThreatMap = {
    //必胜情况
    {22222, PRIORITY_THREAT},// XXXXX
    {12222,PRIORITY_THREAT},// _XXXX
    {22221, PRIORITY_THREAT},// XXXX_
    {22212, PRIORITY_THREAT},// XXX_X
    {21222,PRIORITY_THREAT},//  X_XXX
    {22122, PRIORITY_THREAT},// XX_XX

    // 高威胁棋
    {12221, HIGH_THREAT},
    {12122, HIGH_THREAT},//_X_XX
    {12212, HIGH_THREAT},

    {33331,KILL_POS},//我方杀棋
    {13333,KILL_POS},
    {33313,KILL_POS},
    {31333,KILL_POS},
    {33133,KILL_POS},

    {13331,NOMAL_THREAT},
    {13133,NOMAL_THREAT},
    {13313,NOMAL_THREAT}
};



struct Coordinate{
    int x;
    int y;
    //默认构造函数
    Coordinate() :x(-1), y(-1) {}
    // 带参构造函数
    Coordinate(int row, int col) :x(row), y(col) {}
};
struct ThreatInfo {
    Coordinate position; // 威胁位置
    int weight;          // 权重
};


class Board {
public:
    int board[BOARD_SIZE][BOARD_SIZE] = { {0} };//棋盘布局
    int line_info[2][54] = { {0} };//记录黑子白子在行列对角线的个数，对角线只考虑长度大于等于5的
    bool _checkpos[BOARD_SIZE][BOARD_SIZE] = { false };

    int rowIndex(int row) { return row ; }
    int colIndex(int col) { return 12 + col; }
    int diagIndex(int row, int col) { return 31 - (row - col); }
    int antiDiagIndex(int row, int col) { return 39 + (row + col - 4); }
    void updateInitialLineInfo(int row, int col, int player);
    void undoUpdate(int row, int col, int player);
    void UpdateCheckPos(int x, int y);
    void UndoUpdateCheckPos(int x, int y);
    Board() {
        board[BOARD_MIDDLE_1][BOARD_MIDDLE_1] = WHITE;
        board[BOARD_MIDDLE_2][BOARD_MIDDLE_2] = WHITE;
        board[BOARD_MIDDLE_2][BOARD_MIDDLE_1] = BLACK;
        board[BOARD_MIDDLE_1][BOARD_MIDDLE_2] = BLACK;
        updateInitialLineInfo(BOARD_MIDDLE_1, BOARD_MIDDLE_1, WHITE);
        updateInitialLineInfo(BOARD_MIDDLE_2, BOARD_MIDDLE_2, WHITE);
        updateInitialLineInfo(BOARD_MIDDLE_1, BOARD_MIDDLE_2, BLACK);
        updateInitialLineInfo(BOARD_MIDDLE_2, BOARD_MIDDLE_1, BLACK);
        UpdateCheckPos(BOARD_MIDDLE_1, BOARD_MIDDLE_1);
        UpdateCheckPos(BOARD_MIDDLE_2, BOARD_MIDDLE_2);
        UpdateCheckPos(BOARD_MIDDLE_1, BOARD_MIDDLE_2);
        UpdateCheckPos(BOARD_MIDDLE_2, BOARD_MIDDLE_1);
    }
    void MakeMove(int x, int y, int player)
    {
        board[x][y] = player;
        updateInitialLineInfo(x, y, player);
        _checkpos[x][y] = false;
    }
    void UndoMove(int x, int y) {
        undoUpdate(x, y, board[x][y]);
        board[x][y] = EMPTY;
        _checkpos[x][y] = true;
    }
};
void Board::updateInitialLineInfo(int row, int col, int player) {
    int playerIdx = (player == BLACK) ? 0 : 1;
    line_info[playerIdx][rowIndex(row)]++;
    line_info[playerIdx][colIndex(col)]++;

    // 仅更新长度大于等于 5 的对角线
    if (row - col >= -7 && row - col <= 7) {
        line_info[playerIdx][diagIndex(row, col)]++;
    }
    if (row + col >= 4 && row + col <= 18) {
        line_info[playerIdx][antiDiagIndex(row, col)]++;
    }
}
void Board::undoUpdate(int row, int col, int player) {
    int playerIdx = (player == BLACK) ? 0 : 1;
    line_info[playerIdx][rowIndex(row)]--;
    line_info[playerIdx][colIndex(col)]--;

    // 仅更新长度大于等于 5 的对角线
    if (row - col >= -7 && row - col <= 7) {
        line_info[playerIdx][diagIndex(row, col)]--;
    }
    if (row + col >= 4 && row + col <= 18) {
        line_info[playerIdx][antiDiagIndex(row, col)]--;
    }
}
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
            if (newX >= 0 && newX < BOARD_SIZE && newY >= 0 && newY < BOARD_SIZE) {
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


static bool judgeInRange(Coordinate temp) { //判断位置是否超出棋盘
    if (temp.x < 0)return false;
    if (temp.y < 0)return false;
    if (temp.x > BOARD_SIZE)return false;
    if (temp.y > BOARD_SIZE)return false;
    return true;
}
int PatternRecognize(int count, bool blockedStart, bool blockedEnd, bool gapPattern, int currentPlayer, int player) {
    if (count >= 5) return SCORE_FIVE;  // 五连
    if (count == 4) {
        if (!blockedStart && !blockedEnd) return SCORE_FOUR;         // 活四
        if (!blockedStart || !blockedEnd) return SCORE_BLOCKED_FOUR; // 冲四
    }
    if (count == 3) {
        if(!blockedStart && !blockedEnd && gapPattern) return SCORE_FOUR;
        if ((!blockedStart && !blockedEnd) ) {
            // 如果是对手的活三，则提高防守权重
            return (currentPlayer != player) ? SCORE_THREE * DEFENSE_MULTIPLIER : SCORE_THREE;
        }
        if (!blockedStart || !blockedEnd) {
            return  SCORE_BLOCKED_THREE;
        }
    }
    if (count == 2) {
        if (!blockedStart && !blockedEnd) return SCORE_TWO;  // 活二
        if (!blockedStart || !blockedEnd) return BLOCK_TWO;
    }
    return 0;
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
    int opponent = (player == BLACK) ? WHITE : BLACK;

    // 根据 index 的范围确定遍历方向
    Coordinate temp;
    int dRow, dCol;
    GetDir(temp, index, dRow, dCol);
    int my_count = A.line_info[player == BLACK ? 0 : 1][index];
    int opponent_count = A.line_info[opponent == BLACK ? 0 : 1][index];
    int my_totalcount = 0, opponent_toalcount = 0;
    int line_score = 0;
    while (my_totalcount < my_count || opponent_toalcount < opponent_count) {
        while (judgeInRange(temp)) {
            if (A.board[temp.x][temp.y] != EMPTY)
                break;
            temp.x += dRow;
            temp.y += dCol;
        }//当前棋子位于第一个非空位置
        Coordinate next;
        next.x = temp.x + dRow;
        next.y = temp.y + dCol;
        int count = 0;
        bool start_block = false, gap_pattern = false, end_block = false;
        if (A.board[temp.x][temp.y] == opponent) { // 第一个非空棋子为对方
            start_block = true;
            opponent_toalcount++;
            temp.x += dRow;
            temp.y += dCol;
            next.x += dRow;
            next.y += dCol;
        }
        //
        while (judgeInRange(temp)) {
            if (A.board[temp.x][temp.y] == player) { //第一个非空棋子为我方
                count++;//记录连珠数
                my_totalcount++;
                temp.x += dRow;
                temp.y += dCol;
                next.x += dRow;
                next.y += dCol;

            }
            else if (A.board[temp.x][temp.y] == EMPTY && A.board[next.x][next.y] == player) {
                gap_pattern = true;
                temp.x += dRow;
                temp.y += dCol;
                next.x += dRow;
                next.y += dCol;
            }
            else if (A.board[temp.x][temp.y] == opponent|| !judgeInRange(next)) {
                end_block = true;
                opponent_toalcount++;//边缘有可能多+一个
                break;
            }
            else {
                break;
            }
        }
        lineScore += PatternRecognize(count, start_block, end_block, gap_pattern, player, myFlag);
    }
    return lineScore;
}

// 改进后的评估函数，更强调防守
int evaluateBoard(Board& A, int player) {
    int totalScore = 0;
    int opponent = (player == BLACK) ? WHITE : BLACK;

    // 遍历行、列、对角线的连珠信息
    for (int i = 0; i < 54; i++) {
        if (A.line_info[player == BLACK ? 0 : 1][i] >= 2) {
            int lineScore = CalculateLineScore(A, player, i);
            totalScore += lineScore;
        }
        if (A.line_info[opponent == BLACK ? 0 : 1][i] >= 2) {
            int lineScore = CalculateLineScore(A, opponent, i);
            totalScore -= lineScore;
        }
    }
    return totalScore;
}


bool detectThreats(int player, Board& A, std::vector<Coordinate>& threatPositions) {
    threatPositions.clear();
    // 根据 index 的范围确定遍历方向
    Coordinate temp;
    std::vector<ThreatInfo> threatCandidates;  // 存储所有的威胁信息
    int dRow, dCol;
    for (int i = 0; i < 54; i++) {
        if (A.line_info[player == BLACK ? 0 : 1][i] > 2) {
            GetDir(temp, i, dRow, dCol);
            int binaryPattern = 0;
            int threatLevel = 0;

            std::vector<int> window(5, 0); // 初始化窗口

            // 初始化窗口的前5个位置
            for (int k = 0; k < 5 && judgeInRange(temp); ++k) {
                window[k] = (A.board[temp.x][temp.y] == EMPTY) ? 1 : (A.board[temp.x][temp.y] == player) ? 2 : 3;
                temp.x += dRow;
                temp.y += dCol;
            }

            // 滑动窗口遍历整个行/列/对角线
            while (judgeInRange(temp)) {
                // 将当前窗口转换为10进制棋型编码
                int pattern = 0;
                for (int k = 0; k < 5; ++k) {
                    pattern = pattern * 10 + window[k];
                }

                // 检查该棋型是否存在于威胁表中
                if (ThreatMap.find(pattern) != ThreatMap.end()) {
                    int threatValue = ThreatMap.at(pattern);

                    // 计算威胁下的位置
                    Coordinate threatPosition = temp;
                    if (pattern == 12221 || pattern == 13331) { // _XXX_ 活三时，封堵空位
                        threatPosition.x -= 5 * dRow;
                        threatPosition.y -= 5 * dCol;
                    }
                    else if (pattern == 21221||pattern == 21222||pattern==31331||pattern==31333) { // X_XX_ 时，封堵在第一个空位
                        threatPosition.x -= 4 * dRow;
                        threatPosition.y -= 4 * dCol;
                    }
                    else if (pattern == 12122|| pattern == 22122||pattern==13133||pattern==33133) { // _X_XX 时，封堵在第二个空位
                        threatPosition.x -= 3 * dRow;
                        threatPosition.y -= 3 * dCol;
                    }
                    else if (pattern == 12212|| pattern==22212||pattern==13313||pattern==33313) {
                        threatPosition.x -= 2 * dRow;
                        threatPosition.y -= 2 * dCol;
                    }
                    else if (pattern == 12222||pattern==13333) {
                        threatPosition.x -= 5 * dRow;
                        threatPosition.y -= 5 * dCol;
                    }
                    else if (pattern == 22221||pattern==33331) {
                        threatPosition.x -=  dRow;
                        threatPosition.y -=  dCol;
                    }

                    threatCandidates.push_back({ threatPosition, threatValue });
                }


                // 滑动窗口：移除旧的，添加新的
                for (int k = 0; k < 4; ++k) {
                    window[k] = window[k + 1];
                }
                window[4] = (A.board[temp.x][temp.y] == EMPTY) ? 1 : (A.board[temp.x][temp.y] == player) ? 2 : 3;
                temp.x += dRow;
                temp.y += dCol;
            }
        }
    }
    std::sort(threatCandidates.begin(), threatCandidates.end(), [](const ThreatInfo& a, const ThreatInfo& b) {
        return a.weight > b.weight;
        });

    // 只返回权重最高的威胁位置
    if (!threatCandidates.empty() && A.board[threatCandidates[0].position.x][threatCandidates[0].position.y] == EMPTY) {
        threatPositions.push_back(threatCandidates[0].position);
        return true;
    }

    return false;  // 没有威胁时返回 false
}


int AlphaBeta(Board& A, int depth, int alpha, int beta,int player, Coordinate& bestMove) {
    if (depth == 0) {
        return evaluateBoard(A, player);  // 到达递归深度返回评估值
    }

    if (player == myFlag) {
        for (int row = 0; row < BOARD_SIZE; ++row) {
            for (int col = 0; col < BOARD_SIZE; ++col) {
                if(A._checkpos[row][col])
                {
                    A.MakeMove(row, col, player);

                    int eval = AlphaBeta(A, depth - 1, alpha, beta, enemyFlag, bestMove);

                    if (eval > alpha) {
                        alpha = eval;
                        bestMove = Coordinate(row, col);  // 更新最佳坐标
                    }

                    alpha = std::max(alpha, eval);
                    A.UndoMove(row, col);

                    if (beta <= alpha) {  // Beta 剪枝
                        break;
                    }
                }
            }
        }
        return alpha;
    }
    else {
        for (int row = 0; row < BOARD_SIZE; ++row) {
            for (int col = 0; col < BOARD_SIZE; ++col) {
                if (A._checkpos[row][col])
                {
                    A.MakeMove(row, col, player);
                    int eval = AlphaBeta(A, depth - 1, alpha, beta, myFlag, bestMove);
                    if (eval < beta) {
                        beta = eval;
                        bestMove = Coordinate(row, col);  // 更新最佳坐标
                    }
                    beta = std::min(beta, eval);
                    A.UndoMove(row, col);
                    if (beta <= alpha) {  // Alpha 剪枝
                        break;
                    }
                }
            }
        }
        return beta;
    }
}



void loop()
{
    Board A;
    char tag[10] = { 0 };
    int status;
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
            if (detectThreats(enemyFlag, A, threatPositions)) {
                BestMove = threatPositions.front();
            }
            else{
                AlphaBeta(A, MAX_DEPTH, _INF, INF, myFlag, BestMove);
            }
            A.MakeMove(BestMove.x, BestMove.y, myFlag);
            printf("%d %d\n", BestMove.x, BestMove.y);
            fflush(stdout);
        }

        else if (strcmp(tag, END) == 0) {
            scanf("%d", &status);
            exit(0);
        }

    }
}
int main()
{
    
    loop();
    return 0;
}