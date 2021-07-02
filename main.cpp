#include <iostream>
#include <list>
#include <cmath>

class ToneRow {
private:
    std::list<unsigned int> rowList;
    bool eachElementOccursOnce{};
    unsigned int maskOfRow{};

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

    ToneRow() : ToneRow(0) {
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

    ToneRow& operator=(const ToneRow& rhv) {
        rowList = rhv.rowList;
        maskOfRow = rhv.maskOfRow;
        eachElementOccursOnce = rhv.eachElementOccursOnce;
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& out, const ToneRow& rhv) {
        out << "[ ";
        for(unsigned const int& tone : rhv.rowList) {
            out << tone << " ";
        }
        out << "]";
        return out;
    }
};

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

void FindTwelveToneRow(std::list<std::list<int>>& patternsList, std::list<ToneRow>& result) {
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



int main() {
    std::list<int> pattern = {3, 4};
    std::list<std::list<int>> lstOfPatterns;
    std::list<ToneRow> result;
    for(int i = 0; i < 4; ++i) {
        lstOfPatterns.push_back(pattern);
    }
    FindTwelveToneRow(lstOfPatterns, result);
    if (!result.empty()) {
        for(auto& r : result) {
            std::cout << r << "\n";
        }
        std::cout << std::endl;
    }
    else {
        std::cout << "Серий не нашлось" << std::endl;
    }
}