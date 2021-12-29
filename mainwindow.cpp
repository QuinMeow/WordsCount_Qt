#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , dic()
{
    ui->setupUi(this);
    dic.ui=ui;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_BtnOpenFile_clicked()
{//Open File
    QString curPath = QDir::currentPath();
    QString dlgTitle="打开一个文件";
    QString filter="文本文件(*.txt);;所有文件(*.*)";
    QString aFileName=QFileDialog::getOpenFileName(this,dlgTitle,curPath,filter);
    if(aFileName.isEmpty())
        return;

    dic.LoadFile(aFileName);

}

void MainWindow::on_BtnSearch_clicked()
{
    QString Q_word = ui->lineWord->text();
    dic.Locate(Q_word.toStdString());
}

void Dictionary::Locate(string word) const
{
    QString q_times;
    //QTextStream outstream;
    //outstream.setAutoDetectUnicode(true);
    QString outline;
    int pos = Hash(word);
    int layer = SearchHash(word);
    if (layer != -1)
    {
        q_times = q_times.asprintf("%d",HashTable[pos][layer].location.size());
        ui->labNumbers->setText(q_times);
        vector<pair<int, int>>::iterator location = HashTable[pos][layer].location.begin();
        for (; location != HashTable[pos][layer].location.end(); ++location) //逐个输出位置    
        {
            outline.clear();
            QTextStream(&outline) << "Line " << location->first + 1 << " Word " << location->second + 1;
            ui->listLocations->addItem(outline);
        }
    }
    //else
        //cout << "The word \"" << word << "\" didn't been found!" << endl;
}

void Dictionary::Empty()
{
    map.clear();
    for (int i = 0; i < HASH_SIZE; i++)
        HashTable[i].clear();
}

bool Dictionary::LoadFile(QString &aFileName)
{
    int row = 0, col = 0; //用于统计行列
    QString Q_str;
    string line;
    string word;
    vector<string> v_line;
    regex word_reg("[a-z|A-Z]+"); //无符号字母

    QFile aFile(aFileName);
    if(!aFile.exists())
        return false;
    if(!aFile.open(QIODevice::ReadOnly|QIODevice::Text))
        return false;
    QTextStream aStream(&aFile);
    aStream.setAutoDetectUnicode(true);
    //ui->textOpened->setPlainText(aStream.readAll());

    while (!aStream.atEnd()) //按换行符获取行
    {
        Q_str = aStream.readLine();
        ui->textOpened->append(Q_str);
        line = Q_str.toStdString();
        sregex_token_iterator pos(line.begin(), line.end(), word_reg, 0); //正则匹配获取单词
        decltype(pos) end;
        for (; pos != end; ++pos)
        {
            InsertHash(pos->str(), row, col); //将单词插入哈希表
            col++;
        }
        col = 0; row++;
    }
    return true;
}

int Dictionary::Hash(string& word) const
{
    int seed = 31;
    int hash = 0;
    int strln = word.length();
    for (int i = 0; i < strln; i++)
        hash = (hash * seed + word[i]) % HASH_SIZE; //哈希算法
    return hash % HASH_SIZE;
}


void Dictionary::InsertHash(string word, int row, int col)
{
    bool find = false;
    int pos = Hash(word); //获取单词哈希值
    vector<HashNode>::iterator Point;
    for (Point = HashTable[pos].begin(); Point != HashTable[pos].end(); ++Point) //遍历该哈希值的桶
    {
        if (Point->key == word) //该单词已经出现过
        {
            Point->location.push_back(make_pair(row, col)); //追加位置
            find = true;
            break;
        }
    }
    if (!find) //该单词未出现
    {
        HashTable[pos].push_back(HashNode(word, row, col)); //将该单词放入桶
    }
}

int Dictionary::SearchHash(string& word) const
{
    int layer = 0;
    int pos = Hash(word);
    vector<HashNode>::iterator Point;
    for (Point = HashTable[pos].begin(); Point != HashTable[pos].end(); ++Point) //遍历该哈希值的桶
    {
        if (Point->key == word) //在桶内找到了单词
        {
            return layer; //返回在桶内的层级
        }
        layer++;
    }
    return -1; //没有找到返回-1
}




