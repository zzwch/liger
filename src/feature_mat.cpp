#include <RcppArmadillo.h>
// [[Rcpp::depends(RcppArmadillo)]]
#include <Rcpp.h>

using namespace Rcpp;
using namespace arma;
using namespace std;

// [[Rcpp::export]]
NumericMatrix makeFeatureMatrix(DataFrame& bedmat,StringVector& barcodes){
  NumericMatrix output = NumericMatrix(bedmat.nrows(), barcodes.size());
  output.fill(0);
  vector<string> bedmat_barcodes = as<vector<string> >(bedmat["V7"]);
  rownames(output) = as<StringVector>(bedmat["V4"]);
  colnames(output) = as<StringVector>(barcodes);
  map<string, int> b_to_c;
  vector<string> barcodes_v = as<vector<string>>(barcodes);
  for(int i = 0; i < barcodes_v.size(); ++i){
    b_to_c[barcodes_v[i]] = i;
  }
  set<string> barcode_set(barcodes_v.begin(), barcodes_v.end());
  for(int i = 0; i < bedmat.nrows(); ++i){
    vector<string> associated_barcodes;
    string barcode_string = bedmat_barcodes[i];
    stringstream barcode_list(barcode_string); 
    string intermediate;
    while(getline(barcode_list, intermediate, ';')) { 
      associated_barcodes.push_back(intermediate); 
    }
    for(int j=0; j < associated_barcodes.size(); ++j){
      if (!(barcode_set.find(associated_barcodes[j]) == barcode_set.end())){
        ++output(i,b_to_c[associated_barcodes[j]]);
      }
    }
  }
  return output;
}