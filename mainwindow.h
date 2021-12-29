#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegExp>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>
#include <string>
#include <regex>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

const int HASH_SIZE = 100;
using namespace std;


struct HashNode
{
    string key;
    vector<pair<int, int>> location;
    HashNode(string &a, int row, int col) :key(a) { location.push_back(make_pair(row, col)); }
};

class Dictionary
{
public:
    Ui::MainWindow *ui;
    Dictionary() { HashTable = new vector<HashNode>[HASH_SIZE]; }
    ~Dictionary() {}
    void Count(string word) const; //计数
    void Locate(string word) const; //定位
    void Empty(); //清空
    bool LoadFile(QString &aFileName);

protected:
    vector<vector<string>> map; //邻接矩阵存储
    vector<HashNode>* HashTable; //散列表

    int Hash(string& word) const; //生成哈希值
    void InsertHash(string word, int row, int col); //插入到哈希表并显示
    int SearchHash(string& word) const; //搜索哈希表
    //void Highlight(QString &word) const;
};

class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    Highlighter(QTextDocument *parent = 0);
    void SetText(QString text);
protected:
    void highlightBlock(const QString &text) Q_DECL_OVERRIDE;

    QString word_text;
};
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_BtnOpenFile_clicked();
    void on_BtnSearch_clicked();
private:
    Ui::MainWindow *ui;
    Dictionary dic;
    Highlighter *highlighter;


};




#endif // MAINWINDOW_H

