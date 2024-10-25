#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unordered_set>
#include <vector>
#include <utility>

#include <string>
#include <time.h>

//VERSION1.0:初步实现alphabeta剪枝叶算法，采用先前作业使用过的unordered_set来优化遍历棋盘，原理是把坐标用哈希的形式存储，降低复杂度，同时鉴于五子棋的性质，把检索范围聚焦在有棋位置的
//附近4格（本来想尝试2格，但效果很差）其实用4格已经接近完全遍历了。。但是用哈希存储总会快点。后续优化方向：1.优化存储的方法，应该存在比现有方法更好的遍历方法；2.优化评分标准，注意到
//在评分时棋型分数对最终结果的影响很大，初步想法是针对不同的情况对不同棋型进行动态的赋分，例如先拿黑棋优先攻击？有待继续优化。3.针对棋盘边缘的算法有待优化，提前排除掉完全不可能的值
//将其从表中抹去。
//VERSION1.1：1.观察输的局棋盘，发现自己的算法在面对地方活三时不选择防守而是追求进攻，因此添加防守系数2，在敌方活3时增加3连的倍数；2.同时在面对形如x_xx的棋型时，会被视为活2错误赋分。

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

// 棋型分数
const int SCORE_FIVE = 100000;
const int SCORE_FOUR = 100000;//两头无堵的4连
const int SCORE_BLOCKED_FOUR = 10000;//一端无堵的4连
const int SCORE_THREE = 5000;//两头无堵的3连
const int SCORE_BLOCKED_THREE = 3000;//堵了一边的3连
const int SCORE_TWO = 1000;//两头无堵的2连


const double TIME_LIMIT = 1.8;
const int MAX_DEPTH = 3;

int myFlag;
int enemyFlag;

const int BOARD_MIDDLE_1 = 5;
const int BOARD_MIDDLE_2 = 6;

const int DEFENSE_MULTIPLIER = 2; // 防守优先级加权

// 定义方向向量，用于检测四个方向的连珠情况：水平、垂直、对角线1、对角线2
const int DIRECTIONS[4][2] = { {0, 1}, {1, 0}, {1, 1}, {1, -1} };

struct Coordinate{
    int x;
    int y;
    //默认构造函数
    Coordinate() :x(-1), y(-1) {}
    // 带参构造函数
    Coordinate(int row, int col) :x(row), y(col) {}
};
// 自定义哈希函数，原理是采用std::hash为两个整数生成哈希值，然后将第二个值左移一位（为了让两个值有区分），再两个值异或得到结果
struct PairHash {
    int operator()(const std::pair<int, int>& position) const {
        return std::hash<int>()(position.first) ^ (std::hash<int>()(position.second) << 1);
    }
};
// 采用uorderedser全局变量来存储后续剪枝需要尝试的位置，以达到常数级别的性能
std::unordered_set<std::pair<int, int>, PairHash> CheckPositionSet;

struct Board {
    int board[BOARD_SIZE][BOARD_SIZE] = { {0} };
    int state = 0;
    Board() {
        board[BOARD_MIDDLE_1][BOARD_MIDDLE_1] = WHITE;
        board[BOARD_MIDDLE_2][BOARD_MIDDLE_2] = WHITE;
        board[BOARD_MIDDLE_2][BOARD_MIDDLE_1] = BLACK;
        board[BOARD_MIDDLE_1][BOARD_MIDDLE_2] = BLACK;
    }
    void MakeMove(int x, int y, int player)
    {
        board[x][y] = player;
    }
    void UndoMove(int x, int y) {
        board[x][y] = EMPTY;
    }
};
Board A;

// 添加坐标周围两圈到有效位置集合
void AddSurroundingPositions(int row, int col) {
    for (int i = -4; i <= 4; ++i) {
        for (int j = -4; j <= 4; ++j) {
            int newRow = row + i;
            int newCol = col + j;

            if (newRow >= 0 && newRow < BOARD_SIZE && newCol >= 0 && newCol < BOARD_SIZE && CheckPositionSet.find({ newRow, newCol }) == CheckPositionSet.end() && A.board[newRow][newCol] == EMPTY) {
                CheckPositionSet.insert({ newRow, newCol });
            }
        }
    }
    // 移除当前落子位置
    CheckPositionSet.erase({ row, col });
}
bool judgeInRange(Coordinate temp) { //判断位置是否超出棋盘
    if (temp.x < 0)return false;
    if (temp.y < 0)return false;
    if (temp.x > BOARD_SIZE)return false;
    if (temp.y > BOARD_SIZE)return false;
    return true;
}

// 棋盘局势评估函数
// 根据连线数量和封堵情况为一个方向评分
int calculateLineScore(int count, bool blockedStart, bool blockedEnd, bool gapPattern,int currentPlayer, int player) {
    if (count >= 5) return SCORE_FIVE;  // 五连
    if (count == 4) {
        if (!blockedStart && !blockedEnd) return SCORE_FOUR;         // 活四
        if (!blockedStart || !blockedEnd) return SCORE_BLOCKED_FOUR; // 冲四
    }
    if (count == 3) {
        if ((!blockedStart && !blockedEnd)||gapPattern) {
            // 如果是对手的活三，则提高防守权重
            return (currentPlayer != player) ? SCORE_THREE * DEFENSE_MULTIPLIER : SCORE_THREE;
        }
        if (!blockedStart || !blockedEnd) {
            return (currentPlayer != player) ? SCORE_BLOCKED_THREE : SCORE_BLOCKED_THREE;
        }
    }
    if (count == 2) {
        if (!blockedStart && !blockedEnd) return SCORE_TWO;  // 活二
    }
    return 0;
}


// 检查一个方向上的棋子连线数量和封堵情况
int countConsecutive(int player, int row, int col, int dRow, int dCol, bool& blockedStart, bool& blockedEnd, bool& gapPattern) {
    int count = 1;  // 当前棋子计入连线
    blockedStart = blockedEnd = gapPattern = false;

    // 向一个方向统计连线
    int r = row + dRow, c = col + dCol;
    while (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE && A.board[r][c] == player) {
        count++;
        r += dRow;
        c += dCol;
    }
    if (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE) {
        if (A.board[r][c] == EMPTY) {
            // 检查是否存在 XX_X 形式的活三
            int r2 = r + dRow, c2 = c + dCol;
            if (r2 >= 0 && r2 < BOARD_SIZE && c2 >= 0 && c2 < BOARD_SIZE && A.board[r2][c2] == player) {
                count++;        // 计入该形式的第三个棋子
                gapPattern = true; // XX_X型式的活3
            }
        }
        else {
            blockedEnd = true;
        }
    }
    // 反方向统计
    r = row - dRow;
    c = col - dCol;
    while (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE && A.board[r][c] == player) {
        count++;
        r -= dRow;
        c -= dCol;
    }
    if (A.board[r][c] == EMPTY) {
        int r2 = r - dRow, c2 = c - dCol;
        if (r2 >= 0 && r2 < BOARD_SIZE && c2 >= 0 && c2 < BOARD_SIZE && A.board[r2][c2] == player) {
            count++;
            gapPattern = true;// 检查 X_XX 形式的活3
        }
    }
    else {
        blockedStart = true;
    }

    return count;
}

// 评估整个棋盘局势
int evaluateBoard(int player) {
    int totalScore = 0;
    int opponent = (player == BLACK) ? WHITE :BLACK;

    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            if (A.board[row][col] == player || A.board[row][col] == opponent) {
                int currentPlayer = A.board[row][col];
                int scoreMultiplier = (currentPlayer == player) ? 1 : -1;

                // 对每个方向进行评估
                for (int dir = 0; dir < 4; ++dir) {
                    bool blockedStart = false, blockedEnd = false, gapPattern = false;
                    int count = countConsecutive(currentPlayer, row, col, DIRECTIONS[dir][0], DIRECTIONS[dir][1], blockedStart, blockedEnd, gapPattern);
                    totalScore += scoreMultiplier * calculateLineScore(count, blockedStart, blockedEnd, gapPattern,currentPlayer, myFlag);
                }
            }
        }
    }

    return totalScore;
}

int AlphaBeta(int depth, int alpha, int beta, bool maximizingPlayer, int player, Coordinate& bestMove, clock_t startTime) {
    // 判断是否超过时间限制
    if ((clock() - startTime) / (double)CLOCKS_PER_SEC >= TIME_LIMIT) {
        return evaluateBoard(player);  // 超时返回当前评估值
    }

    if (depth == 0) {
        return evaluateBoard(player);  // 到达递归深度返回评估值
    }

    Coordinate currentBestMove;
    if (maximizingPlayer) {
        int maxEval = _INF;
        for (auto it = CheckPositionSet.begin(); it != CheckPositionSet.end(); ++it) {
            int row = it->first;
            int col = it->second;

            if (A.board[row][col] == EMPTY) {
                A.MakeMove(row, col, player);

                int eval = AlphaBeta( depth - 1, alpha, beta, false, 3 - player, currentBestMove, startTime);

                if (eval > maxEval) {
                    maxEval = eval;
                    bestMove = Coordinate(row, col);  // 更新最佳坐标
                }

                alpha = std::max(alpha, eval);
                A.UndoMove(row, col);

                if (beta <= alpha) break;  // Beta 剪枝
            }
        }
        return maxEval;
    }
    else {
        int minEval = INF;
        for (auto it = CheckPositionSet.begin(); it != CheckPositionSet.end(); ++it) {
            int row = it->first;
            int col = it->second;

            if (A.board[row][col] == EMPTY) {
                A.MakeMove(row, col, player);

                int eval = AlphaBeta( depth - 1, alpha, beta, true, 3 - player, currentBestMove, startTime);

                if (eval < minEval) {
                    minEval = eval;
                    bestMove = Coordinate(row, col);  // 更新最佳坐标
                }

                beta = std::min(beta, eval);
                A.UndoMove(row, col);

                if (beta <= alpha) break;  // Alpha 剪枝
            }
        }
        return minEval;
    }
}


void loop()
{
    char tag[10] = { 0 };
    int status;
    Coordinate command;
    while (1) {
        memset(tag, 0, sizeof(tag));
        scanf("%s", tag);
        if (strcmp(tag, START) == 0) {
            scanf("%d", &myFlag);
            enemyFlag = 3 - myFlag;
            printf("OK\n");
            fflush(stdout);
        }
        else
            if (strcmp(tag, PLACE) == 0) {
                scanf("%d %d", &command.x, &command.y);
                A.board[command.x][command.y] = enemyFlag;
                AddSurroundingPositions(command.x, command.y);
            }
            else if (strcmp(tag, TURN) == 0) {
                Coordinate BestMove;
                clock_t startTime = clock();
                AlphaBeta(MAX_DEPTH, _INF, INF, false, myFlag, BestMove, startTime);
                A.board[BestMove.x][BestMove.y] = myFlag;
                AddSurroundingPositions(BestMove.x, BestMove.y);
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
    AddSurroundingPositions(BOARD_MIDDLE_1, BOARD_MIDDLE_1);
    AddSurroundingPositions(BOARD_MIDDLE_2, BOARD_MIDDLE_2);
    AddSurroundingPositions(BOARD_MIDDLE_1, BOARD_MIDDLE_2);
    AddSurroundingPositions(BOARD_MIDDLE_2, BOARD_MIDDLE_1);
    loop();

    return 0;
}