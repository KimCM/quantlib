/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2000, 2001, 2002, 2003 RiskMap srl

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it
 under the terms of the QuantLib license.  You should have received a
 copy of the license along with this program; if not, please email
 <quantlib-dev@lists.sf.net>. The license is also available online at
 <http://quantlib.org/reference/license.html>.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

/*! \file swaptionvolmatrix.hpp
    \brief Swaption at-the-money volatility matrix
*/

#ifndef quantlib_swaption_volatility_matrix_h
#define quantlib_swaption_volatility_matrix_h

#include <ql/swaptionvolstructure.hpp>
#include <ql/Math/matrix.hpp>
#include <ql/Math/bilinearinterpolation.hpp>
#include <vector>

namespace QuantLib {

    //! At-the-money swaption-volatility matrix
    /*! This class provides the at-the-money volatility for a given
        swaption by interpolating a volatility matrix whose elements
        are the market volatilities of a set of swaption with given
        exercise date and length.

        \warning The volatility matrix must have increasing exercise
                 dates or periods from top to bottom and increasing
                 lenghts from left to right

        \todo either add correct copy behavior or inhibit copy. Right
              now, a copied instance would end up with its own copy of
              the exercise date and length vector but an interpolation
              pointing to the original ones.
    */
    class SwaptionVolatilityMatrix : public SwaptionVolatilityStructure {
      public:
        SwaptionVolatilityMatrix(const Date& referenceDate,
                                 const std::vector<Date>& exerciseDates,
                                 const std::vector<Period>& lengths,
                                 const Matrix& volatilities,
                                 const DayCounter& dayCounter);
        SwaptionVolatilityMatrix(const std::vector<Date>& exerciseDates,
                                 const std::vector<Period>& tenors,
                                 const Matrix& volatilities,
                                 const DayCounter& dayCounter);
        SwaptionVolatilityMatrix(const std::vector<Period>& expiries,
                                 const Calendar& calendar,
                                 const BusinessDayConvention bdc,
                                 const std::vector<Period>& tenors,
                                 const Matrix& volatilities,
                                 const DayCounter& dayCounter);
        // inspectors
        DayCounter dayCounter() const { return dayCounter_; }
        const std::vector<Date>& exerciseDates() const;
        const std::vector<Period>& lengths() const;

		// SwaptionVolatilityStructure interface
		Date maxStartDate() const;
        Time maxStartTime() const;
        Period maxLength() const;
        Time maxTimeLength() const;
        Rate minStrike() const;
        Rate maxStrike() const;
      private:
        DayCounter dayCounter_;
        std::vector<Date> exerciseDates_;
        std::vector<Time> exerciseTimes_;
        std::vector<Period> lengths_;
        std::vector<Time> timeLengths_;
        Matrix volatilities_;
        Interpolation2D interpolation_;
        Volatility volatilityImpl(Time start,
                                  Time length,
                                  Rate strike) const;
        std::pair<Time,Time> convertDates(const Date& start,
                                          const Period& length) const;
    };


    // inline definitions

    inline const std::vector<Date>&
    SwaptionVolatilityMatrix::exerciseDates() const {
        return exerciseDates_;
    }

    inline const std::vector<Period>&
    SwaptionVolatilityMatrix::lengths() const {
        return lengths_;
    }

    inline Date SwaptionVolatilityMatrix::maxStartDate() const {
        return exerciseDates_.back();
    }

    inline Time SwaptionVolatilityMatrix::maxStartTime() const {
        return exerciseTimes_.back();
    }

    inline Period SwaptionVolatilityMatrix::maxLength() const {
        return lengths_.back();
    }

    inline Time SwaptionVolatilityMatrix::maxTimeLength() const {
        return timeLengths_.back();
    }

    inline Rate SwaptionVolatilityMatrix::minStrike() const {
        return QL_MIN_REAL;
    }

    inline Rate SwaptionVolatilityMatrix::maxStrike() const {
        return QL_MAX_REAL;
    }

    inline Volatility SwaptionVolatilityMatrix::volatilityImpl(
                                        Time start, Time length, Rate) const {
        return interpolation_(length,start,true);
    }

}

#endif
