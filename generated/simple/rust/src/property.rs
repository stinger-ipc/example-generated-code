#[allow(unused_imports)]
use crate::payloads::*;
use derive_builder::Builder;

#[derive(Clone, Builder, Debug)]
pub struct SimpleInitialPropertyValues {
    pub school: String,
    pub school_version: u32,
}

#[allow(dead_code)]
impl SimpleInitialPropertyValues {
    /// Validate the initial values of properties that declare a JSON schema constraint.
    ///
    /// Returns `Ok(())` if all schema-constrained properties conform to their schema,
    /// or if no property declares a schema.  Returns `Err` with a description if a
    /// constrained property's value does not conform.
    pub fn validate_schema(&self) -> Result<(), String> {
        Ok(())
    }
}
