#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <utility>
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


const int SCORE_NORMAL = 500;
const int SCORE_SAFE = 100;
const int PRIORITY_THREAT = 10000; // �����в����
const int HIGH_THREAT = 8000; // ����в����
const int NOMAL_THREAT = 1000; //һ����в


const double TIME_LIMIT = 2;
const int MAX_DEPTH = 3;

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
    {12221, PRIORITY_THREAT},// _XXX_

    // ����в��
    {21221, HIGH_THREAT},// X_XX_
    {12122, HIGH_THREAT},//_X_XX

    //�е���в��
    {21212,NOMAL_THREAT},//X_X_X
    {32221,NOMAL_THREAT},//OXXX_
    {12223,NOMAL_THREAT},//_XXXO

    {11222,SCORE_NORMAL},
    {22211,SCORE_NORMAL},



};


struct Coordinate{
    int x;
    int y;
    //Ĭ�Ϲ��캯��
    Coordinate() :x(-1), y(-1) {}
    // ���ι��캯��
    Coordinate(int row, int col) :x(row), y(col) {}
};
// �Զ����ϣ������ԭ���ǲ���std::hashΪ�����������ɹ�ϣֵ��Ȼ�󽫵ڶ���ֵ����һλ��Ϊ��������ֵ�����֣���������ֵ���õ����
struct PairHash {
    int operator()(const std::pair<int, int>& position) const {
        return std::hash<int>()(position.first) ^ (std::hash<int>()(position.second) << 1);
    }
};


class Board {
public:
    int board[BOARD_SIZE][BOARD_SIZE] = { {0} };//���̲���
    int line_info[2][54] = { {0} };//��¼���Ӱ��������жԽ��ߵĸ������Խ���ֻ���ǳ��ȴ��ڵ���5��
    std::unordered_set<std::pair<int, int>, PairHash> CheckPositionSet;

    int rowIndex(int row) { return row - 1; }
    int colIndex(int col) { return 11 + col; }
    int diagIndex(int row, int col) { return 24 + (col - row + 7); }
    int antiDiagIndex(int row, int col) { return 39 + (row + col - 4); }
    void updateInitialLineInfo(int row, int col, int player);
    void undoUpdate(int row, int col, int player);
    void initPosition();
    Board() {
        board[BOARD_MIDDLE_1][BOARD_MIDDLE_1] = WHITE;
        board[BOARD_MIDDLE_2][BOARD_MIDDLE_2] = WHITE;
        board[BOARD_MIDDLE_2][BOARD_MIDDLE_1] = BLACK;
        board[BOARD_MIDDLE_1][BOARD_MIDDLE_2] = BLACK;
        updateInitialLineInfo(BOARD_MIDDLE_1, BOARD_MIDDLE_1, WHITE);
        updateInitialLineInfo(BOARD_MIDDLE_2, BOARD_MIDDLE_2, WHITE);
        updateInitialLineInfo(BOARD_MIDDLE_1, BOARD_MIDDLE_2, BLACK);
        updateInitialLineInfo(BOARD_MIDDLE_2, BOARD_MIDDLE_1, BLACK);
    }
    void MakeMove(int x, int y, int player)
    {
        board[x][y] = player;
        updateInitialLineInfo(x, y, player);
        //CheckPositionSet.erase({ x,y });
    }
    void UndoMove(int x, int y) {
        undoUpdate(x, y, board[x][y]);
        board[x][y] = EMPTY;
        //CheckPositionSet.insert({ x,y });
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
void Board::initPosition(){
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (CheckPositionSet.find({ i, j }) == CheckPositionSet.end() && board[i][j] == EMPTY)
                CheckPositionSet.insert({ i,j });
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

int CalculateLineScore(Board& A,int player, int index) {
    int lineScore = 0;
    int opponent = (player == BLACK) ? WHITE : BLACK;

    // ���� index �ķ�Χȷ����������
    Coordinate temp;
    int dRow, dCol;
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
        if (offset < BOARD_SIZE){
            temp.x = std::max(0, -offset);
            temp.y = std::max(0, offset);
        }
        else{
            offset -= 11;
            temp.x = std::max(0, offset);
            temp.y = std::max(11, -offset);
        }
        dRow = 1;
        dCol = -1;
    }

    // ʹ�� 5 ��λ�õĻ�������������ƥ���ģʽ
    std::vector<int> window(5, 0);  // �洢��ǰ���ڵ� 5 ��λ��
    for (int i = 0; i < 5; i++) {
        window[i] = (A.board[temp.x][temp.y] == EMPTY) ? 1 : (A.board[temp.x][temp.y] == player) ? 2 : 3;
        temp.x += dRow;
        temp.y += dCol;
    }
    while (judgeInRange(temp)) {
        int pattern = 0;
        for (int k = 0; k < 5; ++k) {
            pattern = pattern * 10 + window[k];
        }
        // ���Ҳ����ƥ��ķ���
        if (ThreatMap.find(pattern) != ThreatMap.end()) {
            lineScore += ThreatMap.at(pattern);
        }
        // �ƶ�����
        for (int k = 0; k < 4; ++k) {
            window[k] = window[k + 1];
        }
        window[4] = (A.board[temp.x][temp.y] == EMPTY) ? 1 : (A.board[temp.x][temp.y] == player) ? 2 : 3;
        temp.x += dRow;
        temp.y += dCol;
    }
    
    return lineScore;
}

// �����������̾���
int evaluateBoard(Board& A,int player) {
    int totalScore = 0;
    int opponent = (player == BLACK) ? WHITE : BLACK;

    // �����С��С��Խ��ߵ�������Ϣ
    for (int i = 0; i < 54; i++) {
        if (A.line_info[player == BLACK ? 0 : 1][i] >= 2) {
            int lineScore = CalculateLineScore(A, player, i);
            if (lineScore == 0)
                lineScore = A.line_info[player == BLACK ? 0 : 1][i] * SCORE_NORMAL;
            totalScore += lineScore;
        }
        if(A.line_info[opponent == BLACK ? 0 : 1][i] >= 2) {
            int lineScore = CalculateLineScore(A, opponent, i);
            if (lineScore == 0)
                lineScore = A.line_info[player == BLACK ? 0 : 1][i] * SCORE_NORMAL;
            totalScore -= lineScore;
        }
    }
    return totalScore;
}


int AlphaBeta(Board& A,int depth, int alpha, int beta, bool maximizingPlayer, int player, Coordinate& bestMove) {
    if (depth == 0) {
        return evaluateBoard(A,player);  // ����ݹ���ȷ�������ֵ
    }

    Coordinate currentBestMove;
    if (maximizingPlayer) {
        int maxEval = _INF;
        for (auto it = A.CheckPositionSet.begin(); it != A.CheckPositionSet.end(); ++it) {
            int row = it->first;
            int col = it->second;
            //std::cout << row << "," << col << ":" << maxEval << std::endl;
            if (A.board[row][col] == EMPTY) {
                A.MakeMove(row, col, player);
                int eval = AlphaBeta(A, depth - 1, alpha, beta, false, 3 - player, currentBestMove);

                if (eval > maxEval) {
                    maxEval = eval;
                    bestMove = Coordinate(row, col);  // �����������
                } 

                alpha = std::max(alpha, eval);
                A.UndoMove(row, col);

                if (beta <= alpha) break;  // Beta ��֦
            }
        }
        return maxEval;
    }
    else {
        int minEval = INF;
        for (auto it = A.CheckPositionSet.begin(); it != A.CheckPositionSet.end(); ++it) {
            int row = it->first;
            int col = it->second;

            if (A.board[row][col] == EMPTY) {
                A.MakeMove(row, col, player);

                int eval = AlphaBeta(A, depth - 1, alpha, beta, true, 3 - player, currentBestMove);

                if (eval < minEval) {
                    minEval = eval;
                    bestMove = Coordinate(row, col);  // �����������
                }

                beta = std::min(beta, eval);
                A.UndoMove(row, col);

                if (beta <= alpha) break;  // Alpha ��֦
            }
        }
        return minEval;
    }
}


void loop()
{
    Board A;
    A.initPosition();
    char tag[10] = { 0 };
    int status;
    Coordinate command;
    Coordinate BestMove;
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
                A.CheckPositionSet.erase({ command.x,command.y });
            }
            else if (strcmp(tag, TURN) == 0) {
                AlphaBeta(A,MAX_DEPTH, _INF, INF, false, myFlag, BestMove);
                A.CheckPositionSet.erase({ BestMove.x,BestMove.y });
                A.board[BestMove.x][BestMove.y] = myFlag;
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