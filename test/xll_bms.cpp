// xll_bms.cpp - Black Merton Scholes model
// Implement add-in functions BMS.PUT.VALUE and BMS.PUT.DELTA
// using functions from fms_bms.h.

// Find and cite values from an independent source to verify
// the formulas for the Black Scholes Mertion put value and put delta.

// Create a spreadsheet calling these two functions using
// D = .9, s = 100, sigma = .20, k = 100, and t = 0.25
// Calculate the symmetric difference (v(s + h) - v(s - h))/2h.
// Find the value of h that minimizes the difference from the
// analytic put_delta.