#include <iostream>
#include <list>
#include <cmath>
#include <map>
#include <utility>
#include <vector>
#include <string>

class ToneRow {
private:
    std::list<unsigned int> rowList;
    bool eachElementOccursOnce;
    unsigned int maskOfRow;

    void CalcMaskOfRow() {
        maskOfRow = 0;
        eachElementOccursOnce = true;
        unsigned int maskInThePreviousStep  = 0;
        unsigned int elemOfMask;
        for(unsigned const int& r : rowList) {
            elemOfMask = 1u << r;
            maskOfRow = maskOfRow | elemOfMask;
            if ((maskOfRow == maskInThePreviousStep) && (eachElementOccursOnce == true)) {
                eachElementOccursOnce = false;
            }
            maskInThePreviousStep = maskOfRow;
        }
    }

public:
    explicit ToneRow(unsigned int firstTone) {
        rowList.push_back(firstTone);
        CalcMaskOfRow();
    }

    ToneRow() : ToneRow((unsigned int)0) {

    }

    explicit ToneRow(std::list<unsigned int>& listOfTones) {
        for(unsigned int t : listOfTones) {
            if (t >= 12) {
                t %= 12;
            }
            rowList.push_back(t);
        }
        CalcMaskOfRow();
    }

    explicit ToneRow(std::string row[]) {
        std::map<std::string, unsigned int> isomorphism {
                {"C", 0},
                {"Db", 1},
                {"D", 2},
                { "Eb", 3},
                { "E", 4},
                {"F", 5},
                {"F#", 6},
                {"G", 7},
                {"Ab", 8},
                {"A", 9},
                {"Bb", 10},
                {"B", 11}
        };
        for(int i = 0; i < 12; ++i) {
            rowList.push_back(isomorphism[row[i]]);
        }
        CalcMaskOfRow();
    }

    bool CheckingTheUniquenessOfEachTone() {
        unsigned int maskForCheck = 0;
        unsigned int maskInThePreviousStep  = 0;
        unsigned int elemOfMask;
        for(unsigned const int& r : rowList) {
            elemOfMask = 1u << r;
            maskForCheck = maskForCheck | elemOfMask;
            if (maskForCheck == maskInThePreviousStep) {
                return false;
            }
        }
        return true;
    }

    bool Unique() const {
        return eachElementOccursOnce;
    }

    size_t SizeOfRow() const {
        return rowList.size();
    }

    void AddToneInRow(unsigned const int& newTone) {
        rowList.push_back(newTone);
        unsigned int maskBeforeAdd = maskOfRow;
        unsigned int newToneMask;
        newToneMask = 1u << newTone;
        maskOfRow = maskOfRow | newToneMask;
        if (maskOfRow == maskBeforeAdd) {
            eachElementOccursOnce = false;
        }
    }

    void CalcNewToneByInterval(int interval) {
        int newTone = (int) rowList.back();
        if (abs(interval) >= 12) {
            interval %= 12;
        }
        if (interval < 0) {
            interval += 12;
        }
        newTone += interval;
        newTone %= 12;
        AddToneInRow((unsigned int) newTone);
    }

    ToneRow& operator=(const ToneRow& rhv) = default;

    std::list<unsigned int> GetRowList() const {
        return rowList;
    }

    friend std::ostream& operator<<(std::ostream& out, const ToneRow& rhv) {
        out << "[ ";
        for(unsigned const int& tone : rhv.rowList) {
            out << tone << " ";
        }
        out << "]";
        return out;
    }

    void CalcRowMatrix() {
        std::list<unsigned int> tmpRow = rowList;
        static std::map<unsigned int, std::string> isomorphism = {
                { 0, "C  "},
                { 1, "Db "},
                { 2, "D  "},
                { 3, "Eb "},
                { 4, "E  "},
                { 5, "F  "},
                { 6, "F# "},
                { 7, "G  "},
                { 8, "Ab "},
                { 9, "A  "},
                { 10, "Bb "},
                { 11, "B  "}
        };
        unsigned int rowMtrx[12][12];
        for(size_t i = 0; i < 12; ++i) {
            rowMtrx[0][i] = tmpRow.front();
            tmpRow.pop_front();
        }
        for(size_t j = 1; j < 12; ++j) {
            rowMtrx[j][0] = 12 - rowMtrx[0][j];
        }
        for(size_t k = 1; k < 12; ++k) {
            for(size_t h = 1; h < 12; ++h) {
                rowMtrx[k][h] = (rowMtrx[0][h] + rowMtrx[k][0]) % 12;
            }
        }
        for(auto& k : rowMtrx) {
            for(unsigned int & h : k) {
                std::cout << "| " << isomorphism[h];
            }
            std::cout << "|" << std::endl;
        }
    }

};

class SearchTwelveToneRows {
public:
    std::list<int> pattern;
    std::pair<std::list<ToneRow>,std::list<std::list<int>>> resultOfSearchRows[4];
    std::map<size_t, std::vector<std::string>> textForPrint;

    explicit SearchTwelveToneRows(std::list<int> pttrn) : pattern(std::move(pttrn)) {

        std::list<int> inversionPattern;
        std::list<int> retrogradePattern;
        std::list<int> retrogradeInversionPattern;
        std::list<ToneRow> lstForSearch;
        CreatePatternInversion(inversionPattern);
        CreatePatternRetrograde(retrogradePattern);
        CreatePatternRetrogradeInversion(retrogradeInversionPattern);
        std::list<std::list<int>> arrayOfPatterns[4];
        for (size_t i = 0; i < 2; ++i) {
            arrayOfPatterns[0].push_back(pattern);
            arrayOfPatterns[0].push_back(pattern);

            arrayOfPatterns[1].push_back(pattern);
            arrayOfPatterns[1].push_back(inversionPattern);

            arrayOfPatterns[2].push_back(pattern);
            arrayOfPatterns[2].push_back(retrogradePattern);

            arrayOfPatterns[3].push_back(pattern);
            arrayOfPatterns[3].push_back(retrogradeInversionPattern);
        }
        for(size_t j = 0; j < 4; ++j) {

            resultOfSearchRows[j].first = std::list<ToneRow>();
            resultOfSearchRows[j].second = arrayOfPatterns[j];
        }
        textForPrint[2].emplace_back(std::string("Паттерн + Паттерн + Паттерн + Паттерн = "));
        textForPrint[2].emplace_back(std::string("Паттерн + Инверсия + Паттерн + Инверсия = "));
        textForPrint[2].emplace_back(std::string("Паттерн + Ракоход + Паттерн + Ракод = "));
        textForPrint[2].emplace_back(std::string("Паттерн + Ретроверсия + Паттерн + Ретроверсия = "));

        textForPrint[3].emplace_back(std::string("Паттерн + Паттерн + Паттерн = "));
        textForPrint[3].emplace_back(std::string("Паттерн + Инверсия + Паттерн = "));
        textForPrint[3].emplace_back(std::string("Паттерн + Ракоход + Паттерн = "));
        textForPrint[3].emplace_back(std::string("Паттерн + Ретроверсия + Паттерн = "));

        textForPrint[5].emplace_back(std::string("Паттерн + Паттерн = "));
        textForPrint[5].emplace_back(std::string("Паттерн + Инверсия = "));
        textForPrint[5].emplace_back(std::string("Паттерн + Ракоход = "));
        textForPrint[5].emplace_back(std::string("Паттерн + Ретроверсия = "));

    }

    void CreatePatternInversion(std::list<int>& result) const {
        result = pattern;
        for(int& interval : result) {
            interval *= -1;
        }
    }

    void CreatePatternRetrograde(std::list<int>& result) const {
        result = pattern;
        result.reverse();
    }

    void CreatePatternRetrogradeInversion(std::list<int>& result) const {
        result = pattern;
        CreatePatternInversion(result);
        result.reverse();
    }

    void FindStep(const ToneRow& originalRow, std::list<ToneRow>& result) {
        ToneRow tmp;
        for (int i = 1; i < 12; ++i) {
            tmp = originalRow;
            tmp.CalcNewToneByInterval(i);
            if ((tmp.Unique()) && (tmp.SizeOfRow() <= 12)) {
                result.push_back(tmp);
            }
        }
    }

    bool CheckTwelveToneRow(const ToneRow& row) {
        if ((row.Unique()) && (row.SizeOfRow() == 12)) {
            return true;
        }
        else {
            return false;
        }
    };

    void FindTwelveToneRow(std::list<std::list<int>> patternsList, std::list<ToneRow>& result) {
        std::list<ToneRow> listForFindStep; //сюда буду складывать результаты FindStep
        std::list<ToneRow> listForSearch; //сюда складываю всё, а отсюда уже в result
        std::list<ToneRow> bufferForListForSearch; //сюда временно перекладываю из listForSearch
        ToneRow tmpRow;
        std::list<int> currentPattern;
        int interval;

        listForSearch.push_back(tmpRow);
        currentPattern = patternsList.front();
        patternsList.pop_front();
        do {
            if (!currentPattern.empty()) { //если текущий паттерн ещё ни исчерпан, каждой серии добавляем один его интервал
                interval = currentPattern.front();
                currentPattern.pop_front();
                while(!listForSearch.empty()) {
                    tmpRow = listForSearch.front();
                    listForSearch.pop_front();

                    tmpRow.CalcNewToneByInterval(interval); //рассчитываю новый тон по интервалу
                    if (CheckTwelveToneRow(tmpRow)) { //если серия додекафонная, то добавляю в результат
                        result.push_back(tmpRow);
                    }
                    else if ((tmpRow.Unique()) && (tmpRow.SizeOfRow() < 12)) { //если в неё ещё не 12 тонов, то оставляю для дальнейшего поиска
                        bufferForListForSearch.push_back(tmpRow);
                    }
                }
                listForSearch = bufferForListForSearch;
                bufferForListForSearch.clear();
            }
            else if (currentPattern.empty()) { //если все интервалы из паттерна были добавлены к сериям
                if (!patternsList.empty()) { //обновляем паттерн, если они ещё остались
                    currentPattern = patternsList.front();
                    patternsList.pop_front();
                }
                while(!listForSearch.empty()) {
                    FindStep(listForSearch.front(), listForFindStep);
                    listForSearch.pop_front();
                    while(!listForFindStep.empty()) {
                        if (CheckTwelveToneRow(listForFindStep.front())) {
                            result.push_back(listForFindStep.front());
                            listForFindStep.pop_front();
                        }
                        else {
                            bufferForListForSearch.push_back(listForFindStep.front());
                            listForFindStep.pop_front();
                        }
                    }
                }
                listForSearch = bufferForListForSearch;
                bufferForListForSearch.clear();
            }
            else {
                std::cout << "паттерны закончились" << std::endl;
                break;
            }
        } while(! listForSearch.empty());
    }

    std::string PrintRowByNoteNames(std::list<std::list<int>> lstOfPatterns, const ToneRow& row) {
        static std::map<unsigned int, std::string> noteIsomorphism = {
                { 0, "C"},
                { 1, "Db"},
                { 2, "D"},
                { 3, "Eb"},
                { 4, "E"},
                { 5, "F"},
                { 6, "F#"},
                { 7, "G"},
                { 8, "Ab"},
                { 9, "A"},
                { 10, "Bb"},
                { 11, "B"}
        };
        std::string result;
        std::list<unsigned int> alreadyPrinted;
        std::list<unsigned int> rowForPrint = row.GetRowList();
        int newNote;
        unsigned int currentOctave;


        alreadyPrinted.push_back(rowForPrint.front());
        rowForPrint.pop_front();
        currentOctave = 4;
        result = "[" + noteIsomorphism[alreadyPrinted.back()] + "-" + std::to_string(currentOctave);

        while(alreadyPrinted.size() != 12) {
            if (lstOfPatterns.front().empty() != true) { //если отстраиваю интервал из паттерна
                newNote = (int)(alreadyPrinted.back() + lstOfPatterns.front().front());
                lstOfPatterns.front().pop_front();
                if (newNote >= 12) {
                    ++currentOctave;
                }
                else if (newNote <= 0) {
                    --currentOctave;
                }
                alreadyPrinted.push_back(rowForPrint.front());
                rowForPrint.pop_front();
                result += "," + noteIsomorphism[alreadyPrinted.back()] + "-" + std::to_string(currentOctave);
            }
            else if (lstOfPatterns.front().empty() == true) { //если отстраиваю интервал, найденный поиском
                lstOfPatterns.pop_front();
                currentOctave = 4;
                alreadyPrinted.push_back(rowForPrint.front());
                rowForPrint.pop_front();
                result += "," + noteIsomorphism[alreadyPrinted.back()] + "-" + std::to_string(currentOctave);
            }
        }
        result += "]";
        return result;
    }

    void StartSearch() {
        for(int i = 0; i < 4; ++i) {
            FindTwelveToneRow(resultOfSearchRows[i].second, resultOfSearchRows[i].first);
        }
    }

    void RightWriteCount(size_t& count) {
        if (count < 100) {
            std::cout << "0";
        }
        if (count < 10) {
            std::cout << "0";
        }
        std::cout << count << ") ";
    }

    void PrintAllWhatIFind() {
        size_t countRow = 0;
        size_t typeOfMessage;
        if (pattern.size() >= (size_t)5) {
            typeOfMessage = 5;
        }
        else if ((pattern.size() == (size_t)4) || (pattern.size() == (size_t)3)) {
            typeOfMessage = 3;
        }
        else {
            typeOfMessage = 2;
        }
        for(int i = 0; i < 4; ++i) {
            std::cout << textForPrint[typeOfMessage][i] << resultOfSearchRows[i].first.size() << std::endl;
            for (auto& patt : resultOfSearchRows[i].second) {
                for(auto& p : patt) {
                    if (p > 0) {
                        std::cout << "+" << p << " ";
                    }
                    else {
                        std::cout << p << " ";
                    }
                }
                std::cout << "    ";
            }
            std::cout << std::endl;
            for(auto& r : resultOfSearchRows[i].first) {
                ++countRow;
                RightWriteCount(countRow);
                std::cout << PrintRowByNoteNames(resultOfSearchRows[i].second, r) << std::endl;
            }
            std::cout << "\n\n";
        }
    }
};

int main(int argc, char* argv[]) {

    switch (std::stoi(std::string(argv[1]))) {
        case 1: {
            std::list<int> pattern;
            for (size_t i = 2; i < argc; ++i) {
                pattern.push_back(std::stoi(std::string(argv[i])));
            }
            SearchTwelveToneRows obj(pattern);
            obj.StartSearch();
            obj.PrintAllWhatIFind();
            break;
        }
        case 2: {
            std::string rowStr[12];
            for (size_t i = 2; i < 14; ++i) {
                rowStr[i - 2] = std::string(argv[i]);
            }
            ToneRow row(rowStr);
            row.CalcRowMatrix();
            break;
        }
    }

    return 0;
}