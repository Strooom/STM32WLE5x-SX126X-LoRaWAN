// ######################################################################################
// ### MuMo node : https://github.com/Strooom/MuMo-v2-Node-SW                         ###
// ### Author : Pascal Roobrouck - https://github.com/Strooom                         ###
// ### License : CC 4.0 BY-NC-SA - https://creativecommons.org/licenses/by-nc-sa/4.0/ ###
// ######################################################################################


#pragma once

infoChannels.add(VBAT);
infochannels.add(CPU_TEMP);
if (bme680.ispresent())
		{
	infoChannels.add(bme680::temperature);
	infoChannels.add(bme680::humidity);
	infoChannels.add(bme680::barometricPressure);
		}
if (tsl2591.isPresent())
{
	infoChannels.add(tsl2591::lux1);
	infoChannels.add(tsl2591::lux2);

}


{namespace_end}

