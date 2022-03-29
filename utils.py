import datetime
import numpy as np
from pysolar.solar import *
import pysolar.solartime as stime


def air_mass_correction(zenith_angle: float):
    """Refraction correction for air mass at large zenith angles
    @param zenith_angle: Zenith angle in degrees"""
    angle_rad = zenith_angle * np.pi / 180  # Convert to radians
    cos = np.cos(angle_rad)
    M = (1.002432 * cos ** 2 + 0.148386 * cos + 0.0096467) / \
        (cos ** 3 + 0.149864 * cos ** 2 + 0.0102963 * cos + 0.000303978)
    return M


def format_time(time, delta):
    """Transform from [year, mont, day, hour, minute] format to datetime format"""
    time = datetime.datetime(time[0], time[1], time[2], time[3], time[4], tzinfo=datetime.timezone.utc)
    return time - datetime.timedelta(hours=delta)


def calculate_zenith(time, delta, location):
    """Calculate the zenith angle based on time and location
    @param time: time in [year, mont, day, hour, minute] format
    @param delta: Difference to UTC time
    @param location: location in [latitude, longitude]"""
    return float(90) - get_altitude(location[0], location[1], format_time(time, delta))


def distance_sun_earth(time, delta):
    """Calculate the distance from Earth to the sun in AUs
    @param time: time in [year, mont, day, hour, minute] format
    @param delta: Difference to UTC time"""
    jde = stime.get_julian_ephemeris_day(format_time(time, delta))
    jce = stime.get_julian_ephemeris_century(jde)
    jme = stime.get_julian_ephemeris_millennium(jce)
    return get_sun_earth_distance(jme)
