/*
 * File: mazegraphics.cpp
 * ----------------------
 * @author Julie Zelenski, Fall 2023
 * Implementation of the graphics support routines. You are
 * welcome to read over this code, but you will not need to edit it.
 */
#include "gbutton.h"
#include "glabel.h"
#include "gwindow.h"
#include <iomanip>  // for setw, setfill
#include "error.h"
#include "gthread.h"
#include "map.h"
#include "mazegraphics.h"
using namespace std;


static bool gInitialized = false;
static GWindow *gWindow;
static Map<bool, std::string> gColors;
static GButton *gButton[3];
static GLabel *gLabel;
static bool gPauseForClick;

struct cellT {
    GRect *square;
    GOval *dot;
    bool marked;
};
static Grid<cellT> gCells;

static void unpause(bool noFuturePause = false) {
    gLabel->setColor(gLabel->getBackground());
    gLabel->setEnabled(false);
    gButton[0]->setEnabled(false);
    gButton[1]->setEnabled(false);
    if (noFuturePause) gPauseForClick = false;
}

static void pauseForClick() {
    if (!gInitialized) return;
    gLabel->setColor("Black");
    gButton[0]->setEnabled(true);
    gButton[1]->setEnabled(true);
    while (gButton[0]->isEnabled()) pause(100);
}

static void initialize() {
    gWindow = new GWindow(1, 1);
    gColors[false] = "Dark Gray";
    gColors[true] = "White";
    gWindow->setVisible(false);
    gWindow->setWindowTitle("Maze Graphics");
    gWindow->setRepaintImmediately(false);
    gWindow->setAutoRepaint(false);
    gWindow->setExitOnClose(false);
    gWindow->setLocation(10, 10);
    gWindow->setResizable(false);
    gLabel = new GLabel("Paused...");
    gWindow->addToRegion(gLabel, GWindow::REGION_SOUTH);
    gButton[0] = new GButton("Step");
    gButton[0]->setActionListener([]() { unpause(); });
    gWindow->addToRegion(gButton[0], GWindow::REGION_SOUTH);
    gButton[1] = new GButton("Continue");
    gButton[1]->setActionListener([]() { unpause(true); });
    gWindow->addToRegion(gButton[1], GWindow::REGION_SOUTH);
    unpause();
    gInitialized = true;
}

static void changeDimensions(int numRows, int numCols) {
    static const int kMinWindowSize = 200, kMaxWindowSize = 800;
    static const int kDefaultCellSize = 10;

    if (!gInitialized) initialize();
    gWindow->setVisible(false);
    gWindow->clear();
    gCells.clear();
    gCells.resize(numRows, numCols);
    int cellSize = kDefaultCellSize;
    if (kDefaultCellSize*min(numRows, numCols) < kMinWindowSize)
        cellSize = min(kMinWindowSize/min(numRows, numCols), kMaxWindowSize/max(numRows, numCols));
    int dotSize = int(cellSize * .6);
    int margin = (cellSize - dotSize)/2;
    gWindow->setCanvasSize(numCols*cellSize, numRows*cellSize + 30); // 30 for button in south

    for (const auto& loc : gCells.locations()) {
        gCells[loc].square = new GRect(loc.col * cellSize, loc.row * cellSize, cellSize, cellSize);
        gCells[loc].square->setVisible(false);
        gCells[loc].dot = new GOval(loc.col * cellSize + margin, loc.row * cellSize + margin, dotSize , dotSize);
        gCells[loc].dot->setVisible(false);
        gCells[loc].marked = false;
        gWindow->add(gCells[loc].square);
        gWindow->add(gCells[loc].dot);
    }
}

void drawMaze(const Grid<bool>& g) {
    if (!gInitialized) return;
    for (const auto& loc : gCells.locations()) {
        int val = g[loc];
        gCells[loc].dot->setVisible(false);
        if (val) {
            gCells[loc].square->setVisible(false);
       } else {
            gCells[loc].square->setColor(gColors[val]);
            gCells[loc].square->setFillColor(gColors[val]);
            gCells[loc].square->setVisible(true);
        }
    }
    gWindow->setVisible(true);
    GThread::runOnQtGuiThread([] { gWindow->repaint(); });
}

void initGraphicsForMaze(const Grid<bool>& g, bool pauseOnRedraw) {
    if (g.isEmpty()) error("initGraphicsForMaze asked to display empty grid!");
    if (!gInitialized || g.numRows() != gCells.numRows() || g.numCols() != gCells.numCols()) {
        changeDimensions(g.numRows(), g.numCols());
    }
    gPauseForClick = pauseOnRedraw;
    drawMaze(g);
}

void highlightPath(const Vector<GridLocation>& path, string color) {
    if (!gInitialized) return;

    for (auto& c : gCells) c.marked = false;
    for (GridLocation loc: path) {
        if (!gCells.inBounds(loc)) error("highlightPath asked to highlight path location: " + loc.toString() + " that is out of bounds for drawn grid.");
        gCells[loc].marked = true;
    }
    for (auto& c : gCells) {
        if (c.marked) {
            c.dot->setVisible(true);
            c.dot->setColor(color);
            c.dot->setFillColor(color);
        } else {
            c.dot->setVisible(false);
        }
    }
    GThread::runOnQtGuiThread([] {  gWindow->repaint(); });
    if (gPauseForClick) pauseForClick();

}

void printMaze() {
    if (!gInitialized) return;

    for (const auto& loc : gCells.locations()) {
        char ch = (gCells[loc].square->isVisible()) ? '@' : ' ';
        if (gCells[loc].marked) ch = '+';
        cout << setw(3) << setfill(' ') << ch;
        if (loc.col == gCells.numCols()-1) cout << endl;
    }
}
