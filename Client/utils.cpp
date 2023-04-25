uint8_t checksum(char[] buffer)
{
  uint8_t result = 0;
  for(uint16_t i = 0; buffer[i] != 0; i++)
    result += buffer[i];
  return result % 256;
}

