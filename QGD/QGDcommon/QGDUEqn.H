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
description:
    solvation of momentum equation for QGD solver
source file:
    QGDFoam.C

\*----------------------------------------------------------------------------*/
    solve
    (
        fvm::ddt(rhoU)
        +
        fvc::div(phiJmU)
        +
        fvc::div(phiP)
        -
        fvc::div(phiPi)
    );
    
    // Correct velocity
    U.ref() =
        rhoU()
       /rho();
    U.correctBoundaryConditions();
    
   // Solve diffusive QGD & NS part
    if (implicitDiffusion)
    {
        fvVectorMatrix UEqn
        (
            fvm::ddt(rho, U) - fvc::ddt(rho,U)
            - fvm::laplacian(muf, U)
            - fvc::div(phiTauMC)
        );
        
        solve
        (
            UEqn
        );
        
        rhoU = rho*U;
        
        sigmaDotUPtr() = (muf*linearInterpolate(fvc::grad(U)) + tauMCPtr()) & Uf;
        
        phiSigmaDotU = mesh.Sf() & sigmaDotUPtr(); //or eqn.flux()?
    }
    rhoU.boundaryFieldRef() == rho.boundaryField()*
    U.boundaryField();
    
//
//END-OF-FILE
//


