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
// VERSION 2.4:���������ݹ��л���ֵ��������󣬸����˴������λ�õ����ݽṹ���Ż���alphabeta��֦�߼���ɾ��һ�����������
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
const int PRIORITY_THREAT = 10000; // �����в����
const int HIGH_THREAT = 8000; // ����в����
const int NOMAL_THREAT = 5000; //һ����в


const int SCORE_FIVE = 100000;
const int SCORE_FOUR = 100000;//��ͷ�޶µ�4��
const int SCORE_BLOCKED_FOUR = 50000;//һ���޶µ�4��

const int SCORE_THREE = 10000;//��ͷ�޶µ�3��
const int SCORE_BLOCKED_THREE = 5000;//����һ�ߵ�3��
const int SCORE_TWO = 500;//��ͷ�޶µ�2��
const int BLOCK_TWO = 100;

const double TIME_LIMIT = 2;
const int MAX_DEPTH = 5;

int myFlag;
int enemyFlag;

const int BOARD_MIDDLE_1 = 5;
const int BOARD_MIDDLE_2 = 6;

const int DEFENSE_MULTIPLIER = 10; // �������ȼ���Ȩ


const std::unordered_map<int, int> ThreatMap = {
    //��ʤ���
    {22222, PRIORITY_THREAT},// XXXXX
    {12222,PRIORITY_THREAT},// _XXXX
    {22221, PRIORITY_THREAT},// XXXX_
    {22212, PRIORITY_THREAT},// XXX_X
    {21222,PRIORITY_THREAT},//  X_XXX
    {22122, PRIORITY_THREAT},// XX_XX

    // ����в��
    {12221, HIGH_THREAT},
    {12122, HIGH_THREAT},//_X_XX
    {12212, HIGH_THREAT},

    {33331,KILL_POS},//�ҷ�ɱ��
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
    //Ĭ�Ϲ��캯��
    Coordinate() :x(-1), y(-1) {}
    // ���ι��캯��
    Coordinate(int row, int col) :x(row), y(col) {}
};
struct ThreatInfo {
    Coordinate position; // ��вλ��
    int weight;          // Ȩ��
};


class Board {
public:
    int board[BOARD_SIZE][BOARD_SIZE] = { {0} };//���̲���
    int line_info[2][54] = { {0} };//��¼���Ӱ��������жԽ��ߵĸ������Խ���ֻ���ǳ��ȴ��ڵ���5��
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

    // �����³��ȴ��ڵ��� 5 �ĶԽ���
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

    // �����³��ȴ��ڵ��� 5 �ĶԽ���
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


static bool judgeInRange(Coordinate temp) { //�ж�λ���Ƿ񳬳�����
    if (temp.x < 0)return false;
    if (temp.y < 0)return false;
    if (temp.x > BOARD_SIZE)return false;
    if (temp.y > BOARD_SIZE)return false;
    return true;
}
int PatternRecognize(int count, bool blockedStart, bool blockedEnd, bool gapPattern, int currentPlayer, int player) {
    if (count >= 5) return SCORE_FIVE;  // ����
    if (count == 4) {
        if (!blockedStart && !blockedEnd) return SCORE_FOUR;         // ����
        if (!blockedStart || !blockedEnd) return SCORE_BLOCKED_FOUR; // ����
    }
    if (count == 3) {
        if(!blockedStart && !blockedEnd && gapPattern) return SCORE_FOUR;
        if ((!blockedStart && !blockedEnd) ) {
            // ����Ƕ��ֵĻ���������߷���Ȩ��
            return (currentPlayer != player) ? SCORE_THREE * DEFENSE_MULTIPLIER : SCORE_THREE;
        }
        if (!blockedStart || !blockedEnd) {
            return  SCORE_BLOCKED_THREE;
        }
    }
    if (count == 2) {
        if (!blockedStart && !blockedEnd) return SCORE_TWO;  // ���
        if (!blockedStart || !blockedEnd) return BLOCK_TWO;
    }
    return 0;
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
        }//��ǰ����λ�ڵ�һ���ǿ�λ��
        Coordinate next;
        next.x = temp.x + dRow;
        next.y = temp.y + dCol;
        int count = 0;
        bool start_block = false, gap_pattern = false, end_block = false;
        if (A.board[temp.x][temp.y] == opponent) { // ��һ���ǿ�����Ϊ�Է�
            start_block = true;
            opponent_toalcount++;
            temp.x += dRow;
            temp.y += dCol;
            next.x += dRow;
            next.y += dCol;
        }
        //
        while (judgeInRange(temp)) {
            if (A.board[temp.x][temp.y] == player) { //��һ���ǿ�����Ϊ�ҷ�
                count++;//��¼������
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
                opponent_toalcount++;//��Ե�п��ܶ�+һ��
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

// �Ľ����������������ǿ������
int evaluateBoard(Board& A, int player) {
    int totalScore = 0;
    int opponent = (player == BLACK) ? WHITE : BLACK;

    // �����С��С��Խ��ߵ�������Ϣ
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
    // ���� index �ķ�Χȷ����������
    Coordinate temp;
    std::vector<ThreatInfo> threatCandidates;  // �洢���е���в��Ϣ
    int dRow, dCol;
    for (int i = 0; i < 54; i++) {
        if (A.line_info[player == BLACK ? 0 : 1][i] > 2) {
            GetDir(temp, i, dRow, dCol);
            int binaryPattern = 0;
            int threatLevel = 0;

            std::vector<int> window(5, 0); // ��ʼ������

            // ��ʼ�����ڵ�ǰ5��λ��
            for (int k = 0; k < 5 && judgeInRange(temp); ++k) {
                window[k] = (A.board[temp.x][temp.y] == EMPTY) ? 1 : (A.board[temp.x][temp.y] == player) ? 2 : 3;
                temp.x += dRow;
                temp.y += dCol;
            }

            // �������ڱ���������/��/�Խ���
            while (judgeInRange(temp)) {
                // ����ǰ����ת��Ϊ10�������ͱ���
                int pattern = 0;
                for (int k = 0; k < 5; ++k) {
                    pattern = pattern * 10 + window[k];
                }

                // ���������Ƿ��������в����
                if (ThreatMap.find(pattern) != ThreatMap.end()) {
                    int threatValue = ThreatMap.at(pattern);

                    // ������в�µ�λ��
                    Coordinate threatPosition = temp;
                    if (pattern == 12221 || pattern == 13331) { // _XXX_ ����ʱ����¿�λ
                        threatPosition.x -= 5 * dRow;
                        threatPosition.y -= 5 * dCol;
                    }
                    else if (pattern == 21221||pattern == 21222||pattern==31331||pattern==31333) { // X_XX_ ʱ������ڵ�һ����λ
                        threatPosition.x -= 4 * dRow;
                        threatPosition.y -= 4 * dCol;
                    }
                    else if (pattern == 12122|| pattern == 22122||pattern==13133||pattern==33133) { // _X_XX ʱ������ڵڶ�����λ
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


                // �������ڣ��Ƴ��ɵģ�����µ�
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

    // ֻ����Ȩ����ߵ���вλ��
    if (!threatCandidates.empty() && A.board[threatCandidates[0].position.x][threatCandidates[0].position.y] == EMPTY) {
        threatPositions.push_back(threatCandidates[0].position);
        return true;
    }

    return false;  // û����вʱ���� false
}


int AlphaBeta(Board& A, int depth, int alpha, int beta,int player, Coordinate& bestMove) {
    if (depth == 0) {
        return evaluateBoard(A, player);  // ����ݹ���ȷ�������ֵ
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
                        bestMove = Coordinate(row, col);  // �����������
                    }

                    alpha = std::max(alpha, eval);
                    A.UndoMove(row, col);

                    if (beta <= alpha) {  // Beta ��֦
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
                        bestMove = Coordinate(row, col);  // �����������
                    }
                    beta = std::min(beta, eval);
                    A.UndoMove(row, col);
                    if (beta <= alpha) {  // Alpha ��֦
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