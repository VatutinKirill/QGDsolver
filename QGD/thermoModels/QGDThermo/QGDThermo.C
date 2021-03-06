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

#include "QGDThermo.H"
#include "volFields.H"
#include "surfaceFields.H"
#include "fvMesh.H"
#include "QGDCoeffs.H"

Foam::QGDThermo::QGDThermo(const fvMesh& mesh, const dictionary& dict)
:
mesh_(mesh),
dict_(dict),
qgdCoeffsPtr_
(
    Foam::qgd::QGDCoeffs::New
    (
        dict_.subDict("QGD").lookup("QGDCoeffs"),
        mesh,
        dict_.subDict("QGD")
    )
),
implicitDiffusion_(true)
{
}

Foam::QGDThermo::~QGDThermo()
{

}

bool Foam::QGDThermo::read()
{
    if (dict_.subDict("QGD").found("implicitDiffusion"))
    {
        dict_.subDict("QGD").lookup("implicitDiffusion") >> implicitDiffusion_;
    }
    else
    {
        implicitDiffusion_ = true;
    }
  
  return true;
}

void Foam::QGDThermo::correctQGD(volScalarField& mu, volScalarField& alphau)
{
    qgdCoeffsPtr_->correct(*this);

    const volScalarField& muQGD = this->muQGD();
    const volScalarField& alphauQGD = this->alphauQGD();

    forAll(mu.primitiveField(), celli)
    {
        mu.primitiveFieldRef()[celli] +=
            muQGD.primitiveField()[celli];

        alphau.primitiveFieldRef()[celli] +=
            alphauQGD.primitiveField()[celli];
    }

    forAll(mu.boundaryField(), patchi)
    {
        forAll(mu.boundaryField()[patchi], facei)
        {
            mu.boundaryFieldRef()[patchi][facei] +=
                muQGD.boundaryField()[patchi][facei];

            alphau.boundaryFieldRef()[patchi][facei] +=
                alphauQGD.boundaryField()[patchi][facei];
        }
    }
}

const Foam::volScalarField& Foam::QGDThermo::hQGD() const
{
    return qgdCoeffsPtr_->hQGD();
}

const Foam::volScalarField& Foam::QGDThermo::tauQGD() const
{
    return qgdCoeffsPtr_->tauQGD();
}

const Foam::surfaceScalarField& Foam::QGDThermo::hQGDf() const
{
    return qgdCoeffsPtr_->hQGDf();
}

const Foam::surfaceScalarField& Foam::QGDThermo::tauQGDf() const
{
    return qgdCoeffsPtr_->tauQGDf();
}

const Foam::volScalarField& Foam::QGDThermo::muQGD() const
{
    return qgdCoeffsPtr_->muQGD();
}

const Foam::volScalarField& Foam::QGDThermo::alphauQGD() const
{
    return qgdCoeffsPtr_->alphauQGD();
}

Foam::Switch Foam::QGDThermo::implicitDiffusion() const
{
    return implicitDiffusion_;
}

Foam::qgd::QGDCoeffs& Foam::QGDThermo::qgdCoeffs()
{
  return qgdCoeffsPtr_();
}

//
//END-OF-FILE
//
