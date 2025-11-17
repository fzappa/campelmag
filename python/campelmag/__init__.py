"""Pacote para cálculo de campos elétrico e magnético em linhas de transmissão.

EPRI AC - Transmission Line Reference Book - 200kV and above
Capítulo 7 - Electric and Magnetic Fields
"""

from .campmag import CampelMag

__all__ = ["CampelMag"]
__version__ = "1.0.0"
