#[allow(unused_imports)]
use crate::payloads::*;
use derive_builder::Builder;

#[derive(Clone, Builder, Debug)]
pub struct PropOnlyInitialPropertyValues {
    pub home_address: Address,
    pub home_address_version: u32,

    pub favorite_country: Country,
    pub favorite_country_version: u32,
}

#[allow(dead_code)]
impl PropOnlyInitialPropertyValues {
    /// Validate the initial values of properties that declare a JSON schema constraint.
    ///
    /// Returns `Ok(())` if all schema-constrained properties conform to their schema,
    /// or if no property declares a schema.  Returns `Err` with a description if a
    /// constrained property's value does not conform.
    pub fn validate_schema(&self) -> Result<(), String> {
        Ok(())
    }
}
