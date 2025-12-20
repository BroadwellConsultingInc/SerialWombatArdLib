int errorChecking()
{
	sw.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial

	if(!sw.isLatestFirmware()){Serial.println("Firmware version mismatch.  Download latest Serial Wombat Arduino Library and update Serial Wombat Firmware to latest version");}

	if (!sw.isPinModeSupported(SerialWombatPinMode_t::PIN_MODE_DIGITALIO) )
	{
		Serial.println("The Required Pin mode didn't respond as expected.  Verify proper operation with SerialWombatChipFinder example.");
		if (sw.isSW08())
		{
			Serial.println("You may need to load a different firmware image to your SW8B board.");
		}
		while (1)
		{
			delay(100);
		}

	}

	return(0);
}

