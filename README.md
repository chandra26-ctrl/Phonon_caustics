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
