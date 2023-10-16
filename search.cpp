/*
 * This file contains functions for cleaning and processing tokens,
 * building an inverted index, and performing queries on a database
 * of URLs.
 *
 * @author Gabriel Bo
 * course: CS106B
 * date: 10/16/2023
 */

#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "map.h"
#include "search.h"
#include "set.h"
#include "simpio.h"
#include "strlib.h"
#include "vector.h"
#include "SimpleTest.h"
using namespace std;


/*
 * The cleanToken function takes in one argument, s
 * and it removes all characters that aren't a numerical or letter
 * character in the s string. It returns back the string "cleaned"
 * and lowercased.
 * @param s is the string that will be cleaned
 * @return the string with lowercase font without non-number or letter
 * charaters
 */
string cleanToken(string s)
{
    string result = "";
    string lowercaseS = toLowerCase(s);
    for (int i = 0; i < lowercaseS.length(); i++)
    {
        if (isalpha(lowercaseS[i]) || isdigit(lowercaseS[i]))
        {
            result += lowercaseS[i];
        }
    }
    return result;
}

/*
 * The gatherTokens turns a string of several words into
 * separate tokens. It accesses the stringSplit and cleanToken
 * function to separate each separate word and stores it into a set
 * to ensure that no single token repeats.
 * @param text is the string that can contain more than one separate
 * word that will be converted into multiple tokens
 * @return the set of tokens if the text into lowercased numerical or
 * letter characters
 */
Set<string> gatherTokens(string text) {
    Vector<string> tokenized = stringSplit(text, " ");
    Set<string> tokens;
    for(auto token : tokenized)
    {
        string newToken = cleanToken(token);
        if(!newToken.empty())
        {
            tokens.add(newToken);
        }
    }
    return tokens;
}

/*
 * When using the buildIndex function, it takes in two parameters.
 * The dbfile is an extracted database file that will be processed
 * for it's token characters that align each url link. The map of urls and
 * token characters will be stored in the index argument, which will be used
 * in the next function. And if there are indexes to be built, the function
 * will return the amount of indexes.
 * @param dbfile is the database that will be read
 * @param index is the map of urls with it's aligning tokens that will be filled
 * @return the number of indexes processes and stored into index argument
 */
int buildIndex(string dbfile, Map<string, Set<string>>& index) {
    // Open the database file
    ifstream file(dbfile);
    if(!file.is_open())
    {
        std::cerr << "Error: Database file could not be opened." << std::endl;
        return 0;
    }

    // Read and process each line in the database file
    Map<string, Set<string>> indexPair;
    string url;
    string line;

    while(getline(file, line))
    {
        if(url.empty())
        {
            url = line;
        }
        else
        {
            Set<string> tokens = gatherTokens(line);
            indexPair[url] = tokens;
            url.clear();
        }
    }
    file.close();

    // Update the inverted index for each token
    for(auto i: indexPair)
    {
        string tokenUrl = i;
        Set<string> tokenLine = indexPair.get(i);

        for(string tokenLines : tokenLine)
        {
            index[tokenLines].add(tokenUrl);
        }
    }

    return indexPair.size();
}

/*
 * The findQueryMatches takes in the index argument stored in buildIndex
 * and a string query that is the term that is searched. Using this
 * query string, it will do a search of the database for a set of urls
 * in the form of strings that match with the corresponding input. If there
 * a '+' or '-' modifier, the query search will return different results, intersecting
 * or subtracting the search results respectively.
 * @param index is the index built from the previous function that will be used to
 * find the different urls
 * @param query is the inputed search made by the user
 * @return a set of urls that match the inverted index search
 */
Set<string> findQueryMatches(Map<string, Set<string>>& index, string query)
{
    Set<string> result;
    Vector<string> searchTerms = stringSplit(query, " ");
    string firstTerm = cleanToken(searchTerms[0]);
    if(index.get(firstTerm) != index.get(index.lastKey()))
    {
        result = index[firstTerm];
    }

    for(int i = 1; i < searchTerms.size(); i++)
    {
        char modifier = searchTerms[i][0];
        string term = cleanToken(searchTerms[i].substr(1));

        if(index.get(term) != index.get(index.lastKey()))
        {
            if(modifier == '+')
            {
                // Intersect w/ the matches for term
                Set<string> termSet = index[term];
                result.intersect(termSet);
            }
            else if(modifier == '-')
            {
                // Removes the matches for this term from the current search
                Set<string> termSet = index[term];
                result.difference(termSet);
            }
            else
            {
                // Union w/ the matches for the term
                Set<string> termSet = index[modifier + term];
                result.unionWith(termSet);
            }
        }
    }
    return result;
}

/*
 * The searchEngine function prompts the user to enter a query
 * and returns the search engine results of urls using an inverse index.
 * It takes in a dbfile that is used for the place for searching the index
 * and it's matching url links.
 * @param dbfile contains all the url and index tokens used in the search engine
 * @return void
 */
void searchEngine(string dbfile) {
    // Create the inverted index
    Map<string, Set<string>> index;
    int pageNum = buildIndex(dbfile, index);

    // Print info about the index
    cout << "Processed " << pageNum << " pages containing " << index.size() << " unique terms." << endl;
    cout << endl;

    //Enter a loop for user inputs
    while(true)
    {
        // Prompt user
        string query;
        cout << "Enter query sentence (RETURN/ENTER to quit): ";
        getline(cin, query);

        if(query.empty())
        {
            break;
        }

        // Find and print matching pages for the query
        Set<string> match = findQueryMatches(index, query);

        // Print matching URLs
        cout << "Found " << match.size() << " matching pages" << endl;
        for(auto url : match)
        {
            cout << url << endl;
        }
        cout << endl;
    }
}

/* * * * * * Test Cases * * * * * */

STUDENT_TEST("cleanToken on strings of letters and digits and other characters")
{
    EXPECT_EQUAL(cleanToken("hello--12312314afjaldkf'afa]f[f"), "hello12312314afjaldkfafaff");
    EXPECT_EQUAL(cleanToken("WORLD-c123"), "worldc123");
    EXPECT_EQUAL(cleanToken("CS106B{{{{}}sdfsf"), "cs106bsdfsf");
}

STUDENT_TEST("gatherTokens from string with repeats and uppercase")
{
    Set<string> expected = {"go", "gophers"};
    EXPECT_EQUAL(gatherTokens("go gophers go go GOPHERS"), expected);

    expected = {"i", "love", "cs106b"};
    EXPECT_EQUAL(gatherTokens("!I _lo!@ve_}{}{ C-=+S*106B!"), expected);

    Set<string> tokens = gatherTokens("I lOVe to EaT yuMMY fO0d ** yummy eAt I^^ Love");
    EXPECT_EQUAL(tokens.size(), 6);
    EXPECT(tokens.contains("eat"));
    EXPECT(!tokens.contains("I^^"));
}

STUDENT_TEST("buildIndex is in less that 5 seconds")
{
    Map<string, Set<string>> index;
    TIME_OPERATION(4, buildIndex("res/tiny.txt", index));
}

STUDENT_TEST("findQueryMatches tests operators")
{
    Map<string, Set<string>> index;
    buildIndex("res/website.txt", index);
    Set<string> matches = findQueryMatches(index, "citation");
    EXPECT_EQUAL(matches.size(), 3);
    Set<string> matchesIntersection = findQueryMatches(index, "style +grading");
    EXPECT_EQUAL(matchesIntersection, {"https://cs106b.stanford.edu/about_assignments", "https://cs106b.stanford.edu/assignments/1-cpp/",
                                        "https://cs106b.stanford.edu/assignments/2-adt/", "https://cs106b.stanford.edu/assignments/2-adt/searchengine.html",
                                        "https://cs106b.stanford.edu/lectures/02-cpp/", "https://cs106b.stanford.edu/resources/style_guide.html",
                                        "https://cs106b.stanford.edu/resources/submit_checklist.html", "https://cs106b.stanford.edu/syllabus"}
);
    Set<string> matchesDifference = findQueryMatches(index, "cs106l template -qt");
    EXPECT_EQUAL(matchesDifference.size(), 3);
}


PROVIDED_TEST("gatherTokens from seuss, 6 unique tokens, mixed case, punctuation") {
    Set<string> tokens = gatherTokens("One Fish Two Fish *Red* fish Blue fish ** 10 RED Fish?");
    EXPECT_EQUAL(tokens.size(), 6);
    EXPECT(tokens.contains("fish"));
    EXPECT(!tokens.contains("Fish"));
}

/* Please not add/modify/remove the PROVIDED_TEST entries below.
 * Place your student tests cases above the provided tests.
 */

PROVIDED_TEST("cleanToken on strings of letters and digits") {
    EXPECT_EQUAL(cleanToken("hello"), "hello");
    EXPECT_EQUAL(cleanToken("WORLD"), "world");
    EXPECT_EQUAL(cleanToken("CS106B"), "cs106b");
}

PROVIDED_TEST("cleanToken on strings containing punctuation") {
    EXPECT_EQUAL(cleanToken("/hello/"), "hello");
    EXPECT_EQUAL(cleanToken("~woRLD!"), "world");
    EXPECT_EQUAL(cleanToken("they're"), "theyre");
}

PROVIDED_TEST("cleanToken on string of only punctuation") {
    EXPECT_EQUAL(cleanToken("#$^@@.;"), "");
}

PROVIDED_TEST("gatherTokens from simple string") {
    Set<string> expected = {"go", "gophers"};
    EXPECT_EQUAL(gatherTokens("go go go gophers"), expected);
}

PROVIDED_TEST("gatherTokens correctly cleans tokens") {
    Set<string> expected = {"i", "love", "cs106b"};
    EXPECT_EQUAL(gatherTokens("I _love_ CS*106B!"), expected);
}

PROVIDED_TEST("gatherTokens from seuss, 6 unique tokens, mixed case, punctuation") {
    Set<string> tokens = gatherTokens("One Fish Two Fish *Red* fish Blue fish ** 10 RED Fish?");
    EXPECT_EQUAL(tokens.size(), 6);
    EXPECT(tokens.contains("fish"));
    EXPECT(!tokens.contains("Fish"));
}

PROVIDED_TEST("buildIndex from tiny.txt, 4 pages, 12 unique tokens") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/tiny.txt", index);
    EXPECT_EQUAL(nPages, 4);
    EXPECT_EQUAL(index.size(), 12);
    EXPECT(index.containsKey("fish"));
}

PROVIDED_TEST("findQueryMatches from tiny.txt, single word query") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRed = findQueryMatches(index, "red");
    EXPECT_EQUAL(matchesRed.size(), 2);
    EXPECT(matchesRed.contains("www.dr.seuss.net"));
    Set<string> matchesHippo = findQueryMatches(index, "hippo");
    EXPECT(matchesHippo.isEmpty());
}

PROVIDED_TEST("findQueryMatches from tiny.txt, compound queries") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRedOrFish = findQueryMatches(index, "red fish");
    EXPECT_EQUAL(matchesRedOrFish.size(), 4);
    Set<string> matchesRedAndFish = findQueryMatches(index, "red +fish");
    EXPECT_EQUAL(matchesRedAndFish.size(), 1);
    Set<string> matchesRedWithoutFish = findQueryMatches(index, "red -fish");
    EXPECT_EQUAL(matchesRedWithoutFish.size(), 1);
}
