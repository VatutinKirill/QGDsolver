/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2018 OpenFOAM Foundation
     \\/     M anipulation  | Copyright (C) 2016-2018 OpenCFD Ltd.
-------------------------------------------------------------------------------
                QGDsolver   | Copyright (C) 2016-2018 ISP RAS (www.unicfd.ru)
-------------------------------------------------------------------------------

License
    This file is part of QGDsolver, based on OpenFOAM library.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>. 

\*---------------------------------------------------------------------------*/


#include "byGr.H"
#include "QGDThermo.H"
#include "addToRunTimeSelectionTable.H"
#include "linear.H"
#include "fvCFD.H"


namespace Foam
{
namespace qgd
{
    defineTypeNameAndDebug(byGr,0);
    addToRunTimeSelectionTable
    (
        QGDCoeffs,
        byGr,
        dictionary
    );
}
}

Foam::qgd::
byGr::byGr
(
    const IOobject& io,
    const fvMesh& mesh,
    const dictionary& dict
)
:
    QGDCoeffs(io, mesh, dict),
    Gr_(0.0)
{
    scalar ScQGD = 0.0, PrQGD = 1.0;

    ScQGD_.primitiveFieldRef() = ScQGD;
    PrQGD_.primitiveFieldRef() = PrQGD;
    muQGD_.primitiveFieldRef() = 0.0;
    alphauQGD_.primitiveFieldRef() = 0.0;

    ScQGD_.boundaryFieldRef() = ScQGD;
    PrQGD_.boundaryFieldRef() = PrQGD;
    muQGD_.boundaryFieldRef() = 0.0;
    alphauQGD_.boundaryFieldRef() = 0.0;

  

    dict.lookup("Gr") >> Gr_;
}

Foam::qgd::
byGr::~byGr()
{
}

void Foam::qgd::
byGr::correct(const Foam::QGDThermo& qgdThermo)
{
    this->tauQGD_ = dimensionedScalar("tauQGD",dimTime,1/Gr_);
    this->tauQGDf_= linearInterpolate(this->tauQGD_);
}

//
//END-OF-FILE
//
