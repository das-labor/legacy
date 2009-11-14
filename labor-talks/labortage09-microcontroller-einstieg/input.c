DDRA &= ~(_BV(PA0)); /* PIN A auf Eingang */
if (PINA & _BV(PA0))
{
	/* tue Dinge wenn Eingang high ist */
} else
{
	/* tue andere Dinge */
}
