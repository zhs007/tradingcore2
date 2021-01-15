#include <tradingcore2/csv.h>
#include <tradingcore2/indicator.h>
#include <tradingcore2/utils.h>

#include <functional>

CR2BEGIN

bool parseIndicatorParams(IndicatorParams& params, const char* fullname,
                          const char* assetname) {
  // ema.5
  // ema.29>day/1d/5m/255

  params.clear();

  std::vector<std::string> arr;
  splitStr(arr, fullname, ">");
  if (arr.size() > 0) {
    {
      std::vector<std::string> arr1;
      splitStr(arr1, arr[0].c_str(), ".");
      params.name = arr1[0];

      try {
        params.avgtimes = std::stoi(arr1[1]);
      } catch (...) {
        LOG(ERROR) << "parseIndicatorParams:stoi " << arr1[1];

        return false;
      }
    }

    if (arr.size() > 1) {
      {
        std::vector<std::string> arr1;
        splitStr(arr1, arr[1].c_str(), "/");
        if (arr1.size() != 4) {
          LOG(ERROR) << "parseIndicatorParams:splitStr " << arr[1];

          return false;
        }

        if (arr1[0] == "day") {
          params.b2type = IB2T_DAY;
        } else {
          LOG(ERROR) << "parseIndicatorParams:noday " << arr1[0];

          return false;
        }

        std::vector<std::string> arr2;
        splitStr(arr2, assetname, ".");
        if (arr2.size() > 0) {
          if (arr2.size() == 2) {
            std::vector<std::string> arr3;
            splitStr(arr3, arr2[1].c_str(), "|");
            params.assetsNames.push_back(arr3[0] + "|" + arr1[1]);
            params.assetsNames.push_back(arr3[0] + "|" + arr1[2]);
          } else if (arr2.size() == 1) {
            std::vector<std::string> arr3;
            splitStr(arr3, arr2[0].c_str(), "|");
            params.assetsNames.push_back(arr3[0] + "|" + arr1[1]);
            params.assetsNames.push_back(arr3[0] + "|" + arr1[2]);
          } else {
            LOG(ERROR) << "parseIndicatorParams:splitStr assetname "
                       << assetname;

            return false;
          }
        }

        try {
          params.b2OffTime = std::stoi(arr1[3]);
        } catch (...) {
          LOG(ERROR) << "parseIndicatorParams:stoi2 " << arr1[3];

          return false;
        }
      }
    }
  }

  return true;
}

// saveCSV - save to a csv file
void Indicator::saveCSV(const char* fn) {
  auto onhead = [](FILE* fp) { fprintf(fp, "ts,val\r\n"); };

  auto pIndicator = this;

  auto onrow = [&pIndicator](FILE* fp, int row) {
    if (pIndicator->getLength() <= row) {
      return false;
    }

    fprintf(fp, "%ld,%.4f\r\n", pIndicator->getTimeStamp(row),
            pIndicator->getSingleValue(row)->value);

    return true;
  };

  tr2::saveCSV(fn, onhead, onrow);
}

CR2END