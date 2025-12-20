int errorChecking()
{
	sw6C.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial
	sw6D.registerErrorHandler(SerialWombatSerialErrorHandlerBrief); //Register an error handler that will print communication errors to Serial

	if(!sw6C.isLatestFirmware()){Serial.println("Firmware version mismatch.  Download latest Serial Wombat Arduino Library and update Serial Wombat Firmware to latest version");}
	if(!sw6D.isLatestFirmware()){Serial.println("Firmware version mismatch.  Download latest Serial Wombat Arduino Library and update Serial Wombat Firmware to latest version");}

	if (!sw6C.isPinModeSupported(SerialWombatPinMode_t::PIN_MODE_PULSETIMER) )
	{
		Serial.println("The Required Pin mode didn't respond as expected.  Verify proper operation with SerialWombatChipFinder example.");
		if (sw6C.isSW08())
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

