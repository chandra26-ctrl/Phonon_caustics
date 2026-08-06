# Silicon Wafer Phonon Caustics with G4CMP

This repo is a modified version of the phonon caustic G4CMP example.

This project uses Geant4 and G4CMP to simulate ballistic phonon propagation through a silicon wafer. Phonons are emitted from a point source, transported through the crystal lattice, and recorded when they reach an aluminum sensor on the wafer surface.

## Simulation Geometry
![Visualization of the silicon wafer and aluminum sensor](images/Si_wafer.png)

The simulated geometry consists of:

- A circular silicon wafer (white) with a primary flat
- Wafer radius: `50 mm`
- Wafer thickness: `525 µm`
- Primary-flat length: `32.5 mm`
- Silicon crystal orientation: `(100)` with a `45°` rotation
- A aluminum phonon sensor (blue) covering the upper wafer surface
  
The sensor has approximately the same footprint as the silicon wafer and is positioned directly above its top surface.

The silicon–aluminum boundary is configured for complete phonon absorption. When a phonon reaches this boundary and is absorbed, the simulation records it as a detector hit.

## How Phonons are Spawned
Phonon generation is controlled by `Caustic.mac` and
`Caustic_PhononPrimaryGeneratorAction.cc`.

The default configuration generates:

- `40,000,000` primary phonons
- One phonon per event
- Phonon energy: `0.03 eV`
- Source position: `(0, 0, -0.262 mm)`
- Angular distribution: isotropic within `90° ≤ θ ≤ 180°`

The source is located near one face of the wafer away from the aluminum detector.

## Running the simulation

Building the example using cmake:
```
> mkdir build
> cd build
> cmake ..
> make
```

